#include <script/script.hpp>

#include <backends/cling/cling.hpp>
#include <backends/javascript/js.hpp>

#include <logger/log.hpp>

#include <algorithm>

bool ScriptingInterface::hasCppSupport()
{
#ifdef SCRIPTING_CPP_SUPPORT
    return true;
#else
    return false;
#endif
}

bool ScriptingInterface::hasJavaScriptSupport()
{
#ifdef SCRIPTING_JAVASCRIPT_SUPPORT
    return true;
#else
    return false;
#endif
}

const std::list<Script::Language> &ScriptingInterface::supportedLanguages()
{
    static const std::list<Script::Language> supported = ([]{
        std::list<Script::Language> s;
        if (ScriptingInterface::hasCppSupport())
        {
            s.emplace_back(Script::Cpp);
        }
        if (ScriptingInterface::hasJavaScriptSupport())
        {
            s.emplace_back(Script::JavaScript);
        }
        return s;
    })();

    return supported;
}

Script::Script(Language language)
    : _language(language)
{
    // validate language support
    auto& slangs = ScriptingInterface::supportedLanguages();
    bool supported = (std::find(slangs.begin(), slangs.end(), language) != slangs.end());
    if (!supported)
    {
        throw UnsupportedLanguageBackend("support for this language is not enabled");
    }

    switch (language)
    {
#ifdef SCRIPTING_CPP_SUPPORT
        case Cpp:           this->_script = std::make_shared<ClingCppScript>(); break;
#endif

#ifdef SCRIPTING_JAVASCRIPT_SUPPORT
        case JavaScript:    this->_script = std::make_shared<::JavaScript>(); break;
#endif
    }
}

Script::~Script()
{
}

void Script::setScriptContents(const std::string &script)
{
    if (this->_script)
    {
        this->_script->setScriptContents(script);
    }
}

bool Script::bindValue(const std::string &name, const std::any &value)
{
    if (this->_script)
    {
        return this->_script->bindValue(name, value);
    }
    return false;
}

bool Script::bindFunction(const std::string &name, const std::function<void()> &func)
{
    if (this->_script)
    {
        return this->_script->bindFunction(name, func);
    }
    return false;
}

std::int16_t Script::evaluate()
{
    // evaluate and ignore output
    std::string out;
    return this->evaluate(out);
}

std::int16_t Script::evaluate(std::string &output)
{
    // evaluate and ignore output
    std::string err;
    return this->evaluate(output, err);
}

std::int16_t Script::evaluate(std::string &output, std::string &error)
{
    if (!this->_script)
    {
        // an unsupported language was given in the constructor
        return BackendNotInitialized;
    }

    return this->_script->evaluate(output, error);
}
