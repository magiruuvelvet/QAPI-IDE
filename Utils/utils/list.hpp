#ifndef UTILS_LIST_HPP
#define UTILS_LIST_HPP

#include <algorithm>
#include <list>
#include <map>

#include <utils/string.hpp>

namespace list
{

//
// case insensitive map comparator
//
template<typename KeyType>
struct ci_comparator
{
    bool operator() (const KeyType &k1, const KeyType &k2) const
    {
        return string::to_lower(k1) < string::to_lower(k2);
    }
};

//
// case insensitive multimap, while preserving the actual string case
//
template<typename KeyType, typename ValueType>
using ci_multimap = std::multimap<KeyType, ValueType, ci_comparator<KeyType>>;

//
// check if string map contains a specific key
//
template<typename ListType, typename ListValueType>
bool strcontains_key(const ListType &list, const ListValueType &key,
                     string::case_sensitivity case_sensitivity = string::case_sensitive_compare)
{
    if (case_sensitivity == string::case_sensitive_compare)
    {
        return list.count(key) > 0;
    }
    else
    {
        return std::find_if(list.cbegin(), list.cend(), [&](const auto &item) {
            return string::to_lower(key) == string::to_lower(item.first);
        }) != std::end(list);
    }
}

//
// get all values from a multimap
//
template<typename KeyType, typename ValueType, typename... Comparator>
const std::list<ValueType> mm_values(const std::multimap<KeyType, ValueType, Comparator...> &map, const KeyType &key_name,
                                     string::case_sensitivity case_sensitivity = string::case_sensitive_compare)
{
    std::list<ValueType> values;
    for (auto&& pair : map)
    {
        if (string::compare(pair.first, key_name, case_sensitivity))
        {
            values.emplace_back(pair.second);
        }
    }
    return values;
}

} // namespace list

#endif // UTILS_LIST_HPP
