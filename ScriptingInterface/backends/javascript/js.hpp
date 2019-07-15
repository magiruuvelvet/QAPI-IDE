#ifndef SCRIPTING_JAVASCRIPT_JS_HPP
#define SCRIPTING_JAVASCRIPT_JS_HPP

#include <script/private/scriptprivate.hpp>

class JavaScript final : public ScriptPrivate
{
public:
    JavaScript();
    ~JavaScript() override;

    bool bindValue(const std::string &name, const std::any &value) override;
    bool bindFunction(const std::string &name, const std::function<void()> &func) override;

    std::int16_t evaluate(std::string &output, std::string &error) override;
};

#endif // SCRIPTING_JAVASCRIPT_JS_HPP
