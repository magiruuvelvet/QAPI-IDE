#ifndef LOGGER_SUPPORT_REQUEST_HPP
#define LOGGER_SUPPORT_REQUEST_HPP

#include <fmt/format.h>

#include <requestlib/request.hpp>

template<>
struct fmt::formatter<Request>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const Request &req, FormatContext &ctx)
    {
        return format_to(ctx.out(), "Request{{url=\"{}\", method={}, headers={}}}", req.urlString(), req.method(), req.headers());
    }
};

#endif // LOGGER_SUPPORT_REQUEST_HPP
