#ifndef LOGGER_LOG_HPP
#define LOGGER_LOG_HPP

#include <cstdio>
#include <string>
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

    // FIXME: this should be compile-time constexpr and not run time!
    static const std::map<log_channel, std::string> log_channels;

    static const std::string fmt_log_channel(log_channel channel, const std::string &fmt);

    // default terminal printing of log messages
    // channel format: "CHANNEL: message"
    // none format:    "message"
    template<typename... Arguments>
    constexpr inline static void print(const std::string &message, log_channel channel, fp_t fp, Arguments... args)
    {
        std::fprintf(fp, "%s\n", fmt::format(fmt_log_channel(channel, "{}: ") + message, std::forward<Arguments>(args)...).c_str());
    }

    template<typename... Arguments>
    constexpr inline static void general(const std::string &message, Arguments... args)
    {
        print(message, log_channel::NONE, stdout, args...);
    }

    template<typename... Arguments>
    constexpr inline static void info(const std::string &message, Arguments... args)
    {
        print(message, log_channel::INFO, stdout, args...);
    }

    template<typename... Arguments>
    constexpr inline static void warning(const std::string &message, Arguments... args)
    {
        print(message, log_channel::WARNING, stderr, args...);
    }

    template<typename... Arguments>
    constexpr inline static void error(const std::string &message, Arguments... args)
    {
        print(message, log_channel::ERROR, stderr, args...);
    }

    template<typename... Arguments>
    constexpr inline static void fatal(const std::string &message, Arguments... args)
    {
        print(message, log_channel::FATAL, stderr, args...);
    }

    template<typename... Arguments>
    constexpr inline static void todo(const std::string &message, Arguments... args)
    {
        print(message, log_channel::TODO, stdout, args...);
    }

    template<typename... Arguments>
    inline static const std::string format(const std::string &message, Arguments... args)
    {
        return fmt::format(message, std::forward<Arguments>(args)...);
    }
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
