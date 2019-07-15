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

struct cling_args final
{
    static constexpr char *arg0 = const_cast<char*>("cling");
    static constexpr char *argv[] = {arg0, nullptr};
    static constexpr int argc = 1;
};

} // namespace

ClingCppScript::ClingCppScript()
{
    // FIXME: LLVMDIR should not be hardcoded, but configurable for flexibility
    // FIXME: cling::Interpreter doesn't find its runtime universe if the header
    //        files aren't copied over to the bin directory, this is bu****** and
    //        needs to be fixed by providing the path to the include directory instead
    // NOTE:  adding an -L/path/to/inc option to the cling_args struct causes a SIGSEGV with no backtrace ???

    // create a cling interpreter instance,
    this->interp = std::make_shared<cling::Interpreter>(cling_args::argc, cling_args::argv, LLVMDIR);

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
