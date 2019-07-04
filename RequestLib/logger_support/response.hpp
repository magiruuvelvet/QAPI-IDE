#ifndef LOGGER_SUPPORT_RESPONSE_HPP
#define LOGGER_SUPPORT_RESPONSE_HPP

#include <fmt/format.h>

#include <requestlib/response.hpp>

template<>
struct fmt::formatter<Response>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const Response &res, FormatContext &ctx)
    {
        return format_to(ctx.out(), "Response{{status={}, version={}, headers={}}}",
                         res.status(),
                         res.version().empty() ? "(none)" : res.version(),
                         res.headers());
    }
};

#endif // LOGGER_SUPPORT_RESPONSE_HPP
