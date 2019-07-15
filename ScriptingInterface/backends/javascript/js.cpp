#ifdef SCRIPTING_JAVASCRIPT_SUPPORT

#include "js.hpp"

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

bool JavaScript::bindFunction(const std::string &name, const std::function<void()> &func)
{
    return false;
}

std::int16_t JavaScript::evaluate(std::string &output)
{
    return -255; // backend not initialized
}

#endif // SCRIPTING_JAVASCRIPT_SUPPORT
