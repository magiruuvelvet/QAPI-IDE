#ifndef SCRIPTING_CLING_CLING_HPP
#define SCRIPTING_CLING_CLING_HPP

#include <script/private/scriptprivate.hpp>

#include <memory>

// forward declarations
namespace cling {
    class Interpreter;
}

class ClingCppScript final : public ScriptPrivate
{
public:
    ClingCppScript();
    ~ClingCppScript() override;

    bool bindValue(const std::string &name, const std::any &value) override;
    bool bindVariable(const std::string &name, ScriptVariable &var) override;
    bool bindFunction(const std::string &name, const std::function<void()> &func) override;

    std::int16_t evaluate(std::string &output, std::string &error) override;

private:
    // every script instance has its own interpreter to avoid
    // funny and undefined behavior between scripts
    std::shared_ptr<cling::Interpreter> interp;
};

#endif // SCRIPTING_CLING_CLING_HPP
