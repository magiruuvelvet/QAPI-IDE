#ifndef LOGGER_SUPPORT_MAP_HPP
#define LOGGER_SUPPORT_MAP_HPP

#include <fmt/format.h>

#include <sstream>
#include <map>

#define DECLARE_MAP_FORMAT(map_fmt)                                                         \
template<>                                                                                  \
struct fmt::formatter<map_fmt>                                                              \
{                                                                                           \
    template<typename ParseContext>                                                         \
    constexpr auto parse(ParseContext &ctx)                                                 \
    {                                                                                       \
        return ctx.begin();                                                                 \
    }                                                                                       \
                                                                                            \
    template<typename FormatContext>                                                        \
    auto format(const map_fmt &map, FormatContext &ctx)          \
    {                                                                                       \
        if (map.size() == 0)                                                                \
        {                                                                                   \
            return format_to(ctx.out(), "[]");                                              \
        }                                                                                   \
        else                                                                                \
        {                                                                                   \
            std::stringstream out;                                                          \
            out << "[";                                                                     \
                                                                                            \
            for (auto&& m : map)                                                            \
            {                                                                               \
                out << '"' << m.first << "\": \"" << m.second << "\"; ";                    \
            }                                                                               \
                                                                                            \
            auto out2 = out.str();                                                          \
            out2.erase(out2.size() - 2);                                                    \
            out2 += "]";                                                                    \
                                                                                            \
            return format_to(ctx.out(), "{}", out2);                                        \
        }                                                                                   \
    }                                                                                       \
}

using __map_fmt_string_string = std::map<std::string, std::string>;
using __multimap_fmt_string_string = std::multimap<std::string, std::string>;

DECLARE_MAP_FORMAT(__map_fmt_string_string);
DECLARE_MAP_FORMAT(__multimap_fmt_string_string);

#endif // LOGGER_SUPPORT_MAP_HPP
