#ifndef LOGGER_SUPPORT_QSTRING_HPP
#define LOGGER_SUPPORT_QSTRING_HPP

#include <fmt/format.h>

#include <QString>

template<>
struct fmt::formatter<QString>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const QString &str, FormatContext &ctx)
    {
        return format_to(ctx.out(), "{}", str.toUtf8().constData());
    }
};

#endif // LOGGER_SUPPORT_QSTRING_HPP
