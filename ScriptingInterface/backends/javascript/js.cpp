#ifdef SCRIPTING_JAVASCRIPT_SUPPORT

#include "js.hpp"

#include <script/script.hpp>
#include <script/exceptions.hpp>

#include <logger/log.hpp>

namespace {

} // namespace

JavaScript::JavaScript()
{
}

JavaScript::~JavaScript()
{

}

bool JavaScript::bindValue(const std::string &name, const std::any &value)
{
    return false;
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
    return Script::BackendNotInitialized;
}

#endif // SCRIPTING_JAVASCRIPT_SUPPORT
