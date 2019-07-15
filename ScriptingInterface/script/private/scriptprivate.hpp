#ifndef SCRIPTING_SCRIPTPRIVATE_HPP
#define SCRIPTING_SCRIPTPRIVATE_HPP

#include <string>
#include <cinttypes>
#include <functional>

#include <script/value.hpp>

class ScriptPrivate
{
    friend class Script;

protected:
    ScriptPrivate();
    virtual ~ScriptPrivate();

    void setScriptContents(const std::string &script)
    {
        this->script = script;
    }

    virtual bool bindValue(const std::string &name, const std::any &value) = 0;
    virtual bool bindFunction(const std::string &name, const std::function<void()> &func) = 0;

    virtual std::int16_t evaluate(std::string &output, std::string &error) = 0;

protected:
    std::string script;
};

#endif // SCRIPTING_SCRIPTPRIVATE_HPP
