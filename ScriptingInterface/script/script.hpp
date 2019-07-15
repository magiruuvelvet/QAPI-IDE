#ifndef SCRIPTING_SCRIPT_HPP
#define SCRIPTING_SCRIPT_HPP

#include <script/value.hpp>
#include <script/exceptions.hpp>

#include <memory>
#include <list>
#include <functional>

// this class must not be visible from the outside
class ScriptPrivate;

class Script final
{
public:
    enum Language {
        Cpp,
        JavaScript,
    };

    enum ErrorCode {
        NoError = 0,
        ParseError = -1,
        RuntimeError = -2,
        UnhandledExceptionThrown = -3,
        MemoryAllocationError = -4,
        ScriptContentsEmpty = -5,

        BackendNotInitialized = -255,
    };

    Script(Language language);
    ~Script();

    /**
     * Sets the script contents to evaluate.
     */
    void setScriptContents(const std::string &script);

    /**
     * Binds a static value to the backend. If the value
     * is modified in the script, it won't reflect changes
     * on the C++ side.
     *
     * name is the variable name used in the script
     *
     * throws UnsupportedDataType on incompatible data types
     */
    bool bindValue(const std::string &name, const std::any &value);

    /**
     * Binds a function to the backend.
     *
     * TODO: needs to be designed first without exposing private APIs
     */
    bool bindFunction(const std::string &name, const std::function<void()> &func);

    /**
     * Evaluates the script and returns its exit status.
     * The output is lost when using this method.
     * Positive values are from the script.
     * Negative values indicate errors.
     */
    std::int16_t evaluate();

    /**
     * Evaluates the script and returns its exit status.
     * The output is stored in the output parameter.
     * Positive values are from the script.
     * Negative values indicate errors.
     */
    std::int16_t evaluate(std::string &output);

private:
    Language _language;

    // unique_ptr wants to know sizeof() which is
    // not possible for forward declared types
    // fallback to shared_ptr instead
    std::shared_ptr<ScriptPrivate> _script;
};

struct ScriptingInterface final
{
public:
    static bool hasCppSupport();
    static bool hasJavaScriptSupport();

    static const std::list<Script::Language> &supportedLanguages();

private:
    ScriptingInterface() = delete;
};

#endif // SCRIPTING_SCRIPT_HPP