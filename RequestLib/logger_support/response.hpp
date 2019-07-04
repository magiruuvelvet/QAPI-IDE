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
        // TODO: should this output headers and the response body too? (terminal spam)
        return format_to(ctx.out(), "Response{{status={}, version={}}}", res.status(), res.version());
    }
};

#endif // LOGGER_SUPPORT_RESPONSE_HPP
