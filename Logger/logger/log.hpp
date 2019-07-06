#ifndef LOGGER_LOG_HPP
#define LOGGER_LOG_HPP

#include <cstdio>
#include <string>
#include <string_view>
#include <map>

#include <fmt/format.h>
#include <fmt/printf.h>

// common data types
#include <logger/support/map.hpp>

// TODO: support for files and optimal terminal formatting

// basic logger
struct logger_base final
{
private:
    template<typename... Arguments>
    friend void LOG(Arguments... args);

    template<typename... Arguments>
    friend void LOG_INFO(Arguments... args);

    template<typename... Arguments>
    friend void LOG_WARNING(Arguments... args);

    template<typename... Arguments>
    friend void LOG_ERROR(Arguments... args);

    template<typename... Arguments>
    friend void LOG_FATAL(Arguments... args);

    template<typename... Arguments>
    friend void LOG_TODO(Arguments... args);

    template<typename... Arguments>
    friend const std::string format(Arguments... args);

    enum class log_channel : std::uint8_t {
        NONE,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        TODO,
    };

    using fp_t = decltype(stdout);

    template<log_channel> struct log_channel_config;

#define DECLARE_LOG_CHANNEL(channel)                                       \
    template<> struct log_channel_config<log_channel::channel> {           \
        static const fp_t fp;                                              \
        static constexpr const std::string_view name = #channel;           \
        static constexpr const log_channel channel = log_channel::channel; \
    }

    template<> struct log_channel_config<log_channel::NONE>
    {
        static const fp_t fp;
        static constexpr const std::string_view name = "";
        static constexpr const log_channel channel = log_channel::NONE;
    };

    DECLARE_LOG_CHANNEL(INFO);
    DECLARE_LOG_CHANNEL(WARNING);
    DECLARE_LOG_CHANNEL(ERROR);
    DECLARE_LOG_CHANNEL(FATAL);
    DECLARE_LOG_CHANNEL(TODO);

    // FIXME: this should be compile-time constexpr and not run time!
    static const std::map<const log_channel, const std::string_view> log_channels;

    static const std::string fmt_log_channel(log_channel channel, const std::string_view &fmt);
    static constexpr const std::string_view print_fmt = "{}: ";

    // default terminal printing of log messages
    // channel format: "CHANNEL: message"
    // none format:    "message"
    template<typename... Arguments>
    constexpr inline static void print(const std::string &message, log_channel channel, fp_t fp, Arguments... args)
    {
        std::fprintf(fp, "%s\n", (fmt_log_channel(channel, print_fmt) + fmt::format(message, std::forward<Arguments>(args)...)).c_str());
    }

#define IMPLEMENT_LOG_CHANNEL_PRINT(channel, func_name)                                              \
    template<typename... Arguments>                                                                  \
    constexpr inline static void func_name(const std::string &message, Arguments... args) {          \
        print(message, log_channel::channel, log_channel_config<log_channel::channel>::fp, args...); \
    }

    IMPLEMENT_LOG_CHANNEL_PRINT(NONE,     general)
    IMPLEMENT_LOG_CHANNEL_PRINT(INFO,     info)
    IMPLEMENT_LOG_CHANNEL_PRINT(WARNING,  warning)
    IMPLEMENT_LOG_CHANNEL_PRINT(ERROR,    error)
    IMPLEMENT_LOG_CHANNEL_PRINT(FATAL,    fatal)
    IMPLEMENT_LOG_CHANNEL_PRINT(TODO,     todo)

    template<typename... Arguments>
    inline static const std::string format(const std::string &message, Arguments... args)
    {
        return fmt::format(message, std::forward<Arguments>(args)...);
    }

#undef DECLARE_LOG_CHANNEL
#undef IMPLEMENT_LOG_CHANNEL_PRINT
};


// global logging functions
// use like: LOG_WARNING("hello {}", "world");

template<typename... Arguments>
inline void LOG(Arguments... args)
{
    logger_base::general(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline void LOG_INFO(Arguments... args)
{
    logger_base::info(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline void LOG_WARNING(Arguments... args)
{
    logger_base::warning(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline void LOG_ERROR(Arguments... args)
{
    logger_base::error(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline void LOG_FATAL(Arguments... args)
{
    logger_base::fatal(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline void LOG_TODO(Arguments... args)
{
    logger_base::todo(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline const std::string format(Arguments... args)
{
    return logger_base::format(std::forward<Arguments>(args)...);
}

#endif // LOGGER_LOG_HPP
