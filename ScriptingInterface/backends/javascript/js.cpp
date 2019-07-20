#ifdef SCRIPTING_JAVASCRIPT_SUPPORT

#include "js.hpp"

#include <script/script.hpp>
#include <script/exceptions.hpp>

#include <logger/log.hpp>

#include <duktape-cpp/DuktapeCpp.h>
#include <duktape-cpp/Context.h>

namespace {

template<typename T>
struct type_info final
{
    static constexpr const std::string_view name()
    {
        return typeid(T).name();
    }
};

} // namespace

namespace duk {

#define DUKTAPE_INSPECTABLE_PRIMITIVE(type)     \
    template<>                                  \
    struct Inspect<type> {                      \
        template<class Inspector>               \
        static void inspect(Inspector &) {      \
        }                                       \
    };

DUKTAPE_INSPECTABLE_PRIMITIVE(std::int8_t)
DUKTAPE_INSPECTABLE_PRIMITIVE(std::int16_t)
DUKTAPE_INSPECTABLE_PRIMITIVE(std::int32_t)
DUKTAPE_INSPECTABLE_PRIMITIVE(std::int64_t)
DUKTAPE_INSPECTABLE_PRIMITIVE(std::uint8_t)
DUKTAPE_INSPECTABLE_PRIMITIVE(std::uint16_t)
DUKTAPE_INSPECTABLE_PRIMITIVE(std::uint32_t)
DUKTAPE_INSPECTABLE_PRIMITIVE(std::uint64_t)

template<>
struct Inspect<JavaScript::Console>
{
    template<class Inspector>
    static void inspect(Inspector &i)
    {
        i.method("print", &JavaScript::Console::print);
        i.method("log",   &JavaScript::Console::log);
        i.method("error", &JavaScript::Console::error);
        i.method("warn",  &JavaScript::Console::error);
    }
};

}

DUK_CPP_DEF_CLASS_NAME(JavaScript::Console)

JavaScript::JavaScript()
{
    this->context = std::make_shared<duk::Context>();
    this->console = std::make_shared<Console>();

    this->context->addGlobal("console", this->console);
}

JavaScript::~JavaScript()
{

}

bool JavaScript::bindValue(const std::string &name, const std::any &value)
{
    const auto type = std::string_view{value.type().name()};

#define INTERP_DECLARE_VALUE(tid)                                                                \
    if (type == type_info<tid>::name()) {                                                        \
        this->context->addGlobal(name.c_str(), std::any_cast<tid>(value));                       \
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

    // duktape-cpp doesn't support std::string_view, copy it to a std::string
    else if (type == type_info<std::string_view>::name()) {
        this->context->addGlobal(name.c_str(), std::string{std::any_cast<std::string_view>(value)});
    }

#undef INTERP_DECLARE_VALUE

    else throw UnsupportedDataType(format("duktape: data type not supported: {}", type));

    // error handling not supported by value binding
    return true;
}

bool JavaScript::bindVariable(const std::string &name, ScriptVariable &var)
{
    return false;
}

bool JavaScript::bindFunction(const std::string &name, const std::function<void()> &func)
{
    return false;
}

std::int16_t JavaScript::evaluate(std::string &output, std::string &error)
{
    // wrap script inside a function to handle return codes
    // appends a "return 0;" to the end, if the script returns
    // by itself, the "return 0;" statement is never reached
    // and the scripts return status is used instead
    const std::string script =
            "function duktape_script() { " +
            this->script +
            " return 0; } var duktape_ret_status = duktape_script();";

    Script::ErrorCode interp_status = Script::NoError;
    std::string exception;

    // script return status (JavaScript doesn't support unsigned, so duktape fails on unsigned too)
    int script_return = 0;

    try
    {
        this->context->evalStringNoRes(script.c_str());
        this->context->evalString(script_return, "duktape_ret_status");
    }
    catch (duk::ScriptEvaluationExcepton &e)
    {
        // no error reporting other than an exception
        // extract useful information from the exception
        // message and try to build a proper return
        // status if possible, fallback to UnknownError

        exception = std::string{e.what()};

        if (exception.find("SyntaxError: ") != std::string::npos)
        {
            interp_status = Script::SyntaxError;
        }
        else
        {
            interp_status = Script::UnknownError;
        }
    }

    // obtain console streams
    output = this->console->output_stream.str();
    error = this->console->error_stream.str();

    // append exception message on error and return
    if (interp_status != Script::NoError)
    {
        error += '\n' + exception + '\n';
        return interp_status;
    }

    // return script exit status
    // make it 8-bit unsigned first, than 16-bit signed
    // blame JavaScript not having unsigned values for this
    return static_cast<std::int16_t>(static_cast<std::uint8_t>(script_return));
}



JavaScript::Console::Console()
{
}

JavaScript::Console::~Console()
{
}

void JavaScript::Console::print(const std::string &msg)
{
    this->output_stream << msg << '\n';
}

void JavaScript::Console::log(const std::string &msg)
{
    this->output_stream << msg << '\n';
}

void JavaScript::Console::error(const std::string &msg)
{
    this->error_stream << msg << '\n';
}

void JavaScript::Console::warn(const std::string &msg)
{
    this->error_stream << msg << '\n';
}

#endif // SCRIPTING_JAVASCRIPT_SUPPORT
