// https://stackoverflow.com/a/48820063

// slightly modified to put inside a namespace and make it header friendly

// Example:
// DECLARE_ENUM_WITH_TYPE(TestEnumClass, int32_t, ZERO = 0x00, TWO = 0x02, ONE = 0x01, THREE = 0x03, FOUR);
//
// int main(void) {
//     TestEnumClass first, second;
//     first = TestEnumClass::FOUR;
//     second = TestEnumClass::TWO;
//
//     std::cout << first << "(" << static_cast<uint32_t>(first) << ")" << std::endl; // FOUR(4)
//
//     std::string strOne;
//     strOne = ~first;
//     std::cout << strOne << std::endl; // FOUR
//
//     std::string strTwo;
//     strTwo = ("Enum-" + second) + (TestEnumClass::THREE + "-test");
//     std::cout << strTwo << std::endl; // Enum-TWOTHREE-test
//
//     std::string strThree("TestEnumClass: ");
//     strThree += second;
//     std::cout << strThree << std::endl; // TestEnumClass: TWO
//     std::cout << "Enum count=" << *first << std::endl;
// }

#ifndef ENUM_HPP
#define ENUM_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define ENUM_STRING_REMOVE_CHAR(str, ch) str.erase(std::remove(str.begin(), str.end(), ch), str.end())

namespace ENUM {
    inline std::vector<std::string> splitString(std::string str, char sep = ',') {
        std::vector<std::string> vecString;
        std::string item;

        std::stringstream stringStream(str);

        while (std::getline(stringStream, item, sep))
        {
            vecString.push_back(item);
        }

        return vecString;
    }
}

#define DECLARE_ENUM_WITH_TYPE(E, T, ...)                                                                     \
    enum class E : T                                                                                          \
    {                                                                                                         \
        __VA_ARGS__                                                                                           \
    };                                                                                                        \
    std::map<T, std::string> E##MapName(generateEnumMap<T>(#__VA_ARGS__));                                    \
    std::ostream &operator<<(std::ostream &os, E enumTmp)                                                     \
    {                                                                                                         \
        os << E##MapName[static_cast<T>(enumTmp)];                                                            \
        return os;                                                                                            \
    }                                                                                                         \
    size_t operator*(E enumTmp) { (void) enumTmp; return E##MapName.size(); }                                 \
    std::string operator~(E enumTmp) { return E##MapName[static_cast<T>(enumTmp)]; }                          \
    std::string operator+(std::string &&str, E enumTmp) { return str + E##MapName[static_cast<T>(enumTmp)]; } \
    std::string operator+(E enumTmp, std::string &&str) { return E##MapName[static_cast<T>(enumTmp)] + str; } \
    std::string &operator+=(std::string &str, E enumTmp)                                                      \
    {                                                                                                         \
        str += E##MapName[static_cast<T>(enumTmp)];                                                           \
        return str;                                                                                           \
    }                                                                                                         \
    E operator++(E &enumTmp)                                                                                  \
    {                                                                                                         \
        auto iter = E##MapName.find(static_cast<T>(enumTmp));                                                 \
        if (iter == E##MapName.end() || std::next(iter) == E##MapName.end())                                  \
            iter = E##MapName.begin();                                                                        \
        else                                                                                                  \
        {                                                                                                     \
            ++iter;                                                                                           \
        }                                                                                                     \
        enumTmp = static_cast<E>(iter->first);                                                                \
        return enumTmp;                                                                                       \
    }                                                                                                         \
    bool valid##E(T value) { return (E##MapName.find(value) != E##MapName.end()); }

#define DECLARE_ENUM(E, ...) DECLARE_ENUM_WITH_TYPE(E, int32_t, __VA_ARGS__)
template<typename T>
inline std::map<T, std::string> generateEnumMap(std::string strMap)
{
    ENUM_STRING_REMOVE_CHAR(strMap, ' ');
    ENUM_STRING_REMOVE_CHAR(strMap, '(');

    std::vector<std::string> enumTokens(ENUM::splitString(strMap));
    std::map<T, std::string> retMap;
    T inxMap;

    inxMap = 0;
    for (auto iter = enumTokens.begin(); iter != enumTokens.end(); ++iter)
    {
        // Token: [EnumName | EnumName=EnumValue]
        std::string enumName;
        T enumValue;
        if (iter->find('=') == std::string::npos)
        {
            enumName = *iter;
        }
        else
        {
            std::vector<std::string> enumNameValue(ENUM::splitString(*iter, '='));
            enumName = enumNameValue[0];
            if (std::is_unsigned<T>::value)
            {
                inxMap = static_cast<T>(std::stoull(enumNameValue[1], 0, 0));
            }
            else
            {
                inxMap = static_cast<T>(std::stoll(enumNameValue[1], 0, 0));
            }
        }
        retMap[inxMap++] = enumName;
    }

    return retMap;
}

#endif // ENUM_HPP
