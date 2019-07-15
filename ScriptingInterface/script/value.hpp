#ifndef SCRIPTING_SCRIPTVALUE_HPP
#define SCRIPTING_SCRIPTVALUE_HPP

#include <cstdint>
#include <any>

struct ScriptVariable final
{
    std::any value;
    const uintptr_t address = reinterpret_cast<uintptr_t>(&value);
};

#endif // SCRIPTING_SCRIPTVALUE_HPP
