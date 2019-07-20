#ifndef SCRIPTING_JAVASCRIPT_JS_HPP
#define SCRIPTING_JAVASCRIPT_JS_HPP

#include <script/private/scriptprivate.hpp>

#include <memory>
#include <sstream>

// forward declarations
namespace duk {
    class Context;
}

class JavaScript final : public ScriptPrivate
{
public:
    JavaScript();
    ~JavaScript() override;

    bool bindValue(const std::string &name, const std::any &value) override;
    bool bindVariable(const std::string &name, ScriptVariable &var) override;
    bool bindFunction(const std::string &name, const std::function<void()> &func) override;

    std::int16_t evaluate(std::string &output, std::string &error) override;

    // console object
    //
    //  js usage:
    //    console.print("message");
    //    console.error("error message");
    class Console
    {
    public:
        explicit Console();
        ~Console();

        // standard output stream
        void print(const std::string &msg);
        void log(const std::string &msg);     // for compatibility
        std::stringstream output_stream;

        // standard error stream
        void error(const std::string &msg);
        void warn(const std::string &msg);    // for compatibility
        std::stringstream error_stream;
    };

private:
    std::shared_ptr<duk::Context> context;
    std::shared_ptr<Console> console;
};

#endif // SCRIPTING_JAVASCRIPT_JS_HPP
