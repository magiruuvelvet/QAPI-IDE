#ifndef LOGGER_SUPPORT_URL_HPP
#define LOGGER_SUPPORT_URL_HPP

#include <fmt/format.h>

#include <url.h>

template<>
struct fmt::formatter<Url::Url>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const Url::Url &res, FormatContext &ctx)
    {
        return format_to(ctx.out(), "Url{{raw=\"{}\"}}", res.str());
    }
};

#endif // LOGGER_SUPPORT_URL_HPP
