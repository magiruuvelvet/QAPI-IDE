#ifndef UI_CONSTANTS_HPP
#define UI_CONSTANTS_HPP

#include <string_view>

struct app final
{
    static constexpr const std::string_view name = "QAPI-IDE";
    static constexpr const std::string_view prettyName = "QApi IDE";
    static constexpr const std::string_view version = "draft";
};

#endif // UI_CONSTANTS_HPP
