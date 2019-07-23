#ifdef SCRIPTING_CPP_SUPPORT

#include "cling.hpp"

#include <script/script.hpp>
#include <script/exceptions.hpp>

#include <logger/log.hpp>

#include <cling/Interpreter/Interpreter.h>
#include <cling/Interpreter/Value.h>

namespace {

template<typename T>
struct type_info final
{
    static constexpr const std::string_view name()
    {
        return typeid(T).name();
    }
};

// simulates argc and **argv with RAII
// no need to manually clean up
// modifying argv() causes undefined behavior, because the underlying data is read-only
struct argv_t final
{
public:
    template<typename... Arguments>
    constexpr argv_t(Arguments... args)
    {
        this->args = std::vector<std::string_view>{args...};
    }

    constexpr void append(const std::string_view &argument)
    {
        this->args.emplace_back(argument);
    }

    constexpr int argc() const
    {
        return int(args.size());
    }

    char **argv() const
    {
        c_argv.clear();
        std::transform(args.begin(), args.end(), std::back_inserter(c_argv), [](const std::string_view &arg) {
            return const_cast<char*>(arg.data());
        });
        return c_argv.data();
    }

private:
    std::vector<std::string_view> args;
    mutable std::vector<char*> c_argv;
};

static argv_t cling_args{
    "cling",
    "-std=c++1z",
    "-L" LLVMDIR "/include",
    "-I" LLVMDIR "/include",

    // TODO: cache using `-include-pch` to heavily speed up execution
    // from ~500ms down to ~2ms per script
    // this needs to be generated during the first script construction
    // and reused afterwards
    // (clang++ -std=c++1z header-cache.hpp -o header-cache.pch)
};

} // namespace

ClingCppScript::ClingCppScript()
{
    // FIXME: LLVMDIR should not be hardcoded, but configurable for flexibility

    // create a cling interpreter instance,
    this->interp = std::make_shared<cling::Interpreter>(cling_args.argc(), cling_args.argv(), LLVMDIR);

    // load common headers
    this->interp->declare("#include <iostream>");
    this->interp->declare("#include <string>");
    this->interp->declare("#include <string_view>");
    this->interp->declare("#include <sstream>");
    this->interp->declare("#include <vector>");
    this->interp->declare("#include <list>");
    this->interp->declare("#include <cstdint>");
    this->interp->declare("#include <cstdio>");
    this->interp->declare("#include <cstdlib>");
    this->interp->declare("#include <algorithm>");
    this->interp->declare("#include <functional>");
    this->interp->declare("#include <any>");
    this->interp->declare("#include <cmath>");

    // global output buffer
    this->interp->declare("std::stringstream out;");
    this->interp->declare("std::stringstream err;");
}

ClingCppScript::~ClingCppScript()
{
}

bool ClingCppScript::bindValue(const std::string &name, const std::any &value)
{
    const auto type = std::string_view{value.type().name()};
    cling::Interpreter::CompilationResult res = {};

#define INTERP_DECLARE_VALUE(tid)                                                                \
    if (type == type_info<tid>::name()) {                                                        \
        res = this->interp->declare(format(#tid " {} = {};", name, std::any_cast<tid>(value)));  \
    }
#define INTERP_DECLARE_VALUE_STR(tid)                                                                \
    if (type == type_info<tid>::name()) {                                                            \
        res = this->interp->declare(format(#tid " {} = \"{}\";", name, std::any_cast<tid>(value)));  \
    }

         INTERP_DECLARE_VALUE(std::int8_t)
    else INTERP_DECLARE_VALUE(std::int16_t)
    else INTERP_DECLARE_VALUE(std::int32_t)
    else INTERP_DECLARE_VALUE(std::int64_t)

    else INTERP_DECLARE_VALUE(std::uint8_t)
    else INTERP_DECLARE_VALUE(std::uint16_t)
    else INTERP_DECLARE_VALUE(std::uint32_t)
    else INTERP_DECLARE_VALUE(std::uint64_t)

    else INTERP_DECLARE_VALUE_STR(std::string)
    else INTERP_DECLARE_VALUE_STR(std::string_view)

#undef INTERP_DECLARE_VALUE
#undef INTERP_DECLARE_VALUE_STR

    else throw UnsupportedDataType(format("cling: data type not supported: {}", type));

    return res == cling::Interpreter::kSuccess;
}

bool ClingCppScript::bindVariable(const std::string &name, ScriptVariable &var)
{
    const auto type = std::string_view{var.value.type().name()};
    cling::Interpreter::CompilationResult res = {};

    // FIXME: is is not working as it should. memory access violation problem

    // type val = std::any_cast<type>(*reinterpret_cast<std::any*>(a.address));
#define INTERP_DECLARE_VALUE(tid)                                                                \
    if (type == type_info<tid>::name()) {                                                        \
        res = this->interp->declare(format("uintptr_t cling_bind_var_addr_{} = {};"              \
                                           "std::any *{} = reinterpret_cast<std::any*>({});",    \
                                           name, var.address, name, var.address));               \
    }

         INTERP_DECLARE_VALUE(std::int8_t)
    else INTERP_DECLARE_VALUE(std::int16_t)
    else INTERP_DECLARE_VALUE(std::int32_t)
    else INTERP_DECLARE_VALUE(std::int64_t)

    else INTERP_DECLARE_VALUE(std::uint8_t)
    else INTERP_DECLARE_VALUE(std::uint16_t)
    else INTERP_DECLARE_VALUE(std::uint32_t)
    else INTERP_DECLARE_VALUE(std::uint64_t)

    else INTERP_DECLARE_VALUE(std::string)
    else INTERP_DECLARE_VALUE(std::string_view)

#undef INTERP_DECLARE_VALUE

    else throw UnsupportedDataType(format("cling: data type not supported: {}", type));

    return res == cling::Interpreter::kSuccess;
}

bool ClingCppScript::bindFunction(const std::string &name, const std::function<void()> &func)
{
    return false;
}

std::int16_t ClingCppScript::evaluate(std::string &output, std::string &error)
{
    // wrap script inside a function to handle return codes
    // appends a "return 0;" to the end, if the script returns
    // by itself, the "return 0;" statement is never reached
    // and the scripts return status is used instead
    const std::string script =
            "std::uint8_t cling_script() { " +
            this->script +
            " return 0; } auto cling_ret_status = cling_script();";

    // execute C++ code
    const auto res = this->interp->process(script, nullptr, nullptr, true);

    // get return status
    cling::Value val;
    this->interp->process("cling_ret_status;", &val, nullptr, true);
    const auto ret = val.simplisticCastAs<std::uint8_t>();

    // get output from globally declared "out" buffer
    this->interp->process("out.str();", &val, nullptr, true);
    output = std::string{*reinterpret_cast<const std::string*>(val.getPtr())};
    this->interp->process("err.str();", &val, nullptr, true);
    error = std::string{*reinterpret_cast<const std::string*>(val.getPtr())};

    // NOTE: cling can't report parse or compile errors
    switch (res)
    {
        case cling::Interpreter::kSuccess: return static_cast<std::int16_t>(ret);
        case cling::Interpreter::kFailure: return Script::RuntimeError;
        case cling::Interpreter::kMoreInputExpected: return Script::ParseError;
    }
}

#endif // SCRIPTING_CPP_SUPPORT
