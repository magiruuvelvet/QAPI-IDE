#ifndef UTILS_STRING_HPP
#define UTILS_STRING_HPP

#include <algorithm>

namespace string
{

//
// ASCII to lower case conversion
//
template<typename StringType>
const StringType to_lower(const StringType &str)
{
    StringType rw_copy = str;
    std::transform(rw_copy.begin(), rw_copy.end(), rw_copy.begin(), ::tolower);
    return rw_copy;
}

//
// ASCII to upper case conversion
//
template<typename StringType>
const StringType to_upper(const StringType &str)
{
    StringType rw_copy = str;
    std::transform(rw_copy.begin(), rw_copy.end(), rw_copy.begin(), ::toupper);
    return rw_copy;
}

//
// case sensitivity mode
//
enum case_sensitivity
{
    case_sensitive_compare,
    case_insensitive_compare,
};

//
// string comparison with case sensitivity toggle
//
template<typename StringType1, typename StringType2>
bool compare(const StringType1 &str1, const StringType2 &str2, case_sensitivity case_sensitivity_ = case_sensitive_compare)
{
    if (case_sensitivity_ == case_sensitive_compare)
    {
        return str1 == str2;
    }
    else
    {
        return to_lower(str1) == to_lower(str2);
    }
}

} // namespace string

#endif // UTILS_STRING_HPP
