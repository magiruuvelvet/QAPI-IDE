#ifndef LOGGER_LOG_HPP
#define LOGGER_LOG_HPP

#include <iostream>
#include <sstream>
#include <memory>

#include <fmt/format.h>
#include <fmt/printf.h>

// basic logger

// for general string formatting use fmt directly like so
//  std::string fmt::format("...", ...)
// doesn't support printf-like syntax

// or preferably with printf-like syntax
//  std::string format("%s", "hello");

struct logger_base final
{
    static auto *self()
    {
        return _self.get();
    }

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
    friend std::string format(Arguments... args);

private:
    static std::unique_ptr<logger_base> _self;

    enum class log_channel {
        NONE,
        INFO,
        WARNING,
        ERROR,
        FATAL,
    };

    // TODO: support for files and optimal terminal formatting

    template<typename... Arguments>
    inline void general(const std::string &message, Arguments... args)
    {
        fmt::fprintf(std::cout, message + "\n", std::forward<Arguments>(args)...);
    }

    template<typename... Arguments>
    inline void info(const std::string &message, Arguments... args)
    {
        fmt::fprintf(std::cout, "INFO: " + message + "\n", std::forward<Arguments>(args)...);
    }

    template<typename... Arguments>
    inline void warning(const std::string &message, Arguments... args)
    {
        fmt::fprintf(std::cerr, "WARNING: " + message + "\n", std::forward<Arguments>(args)...);
    }

    template<typename... Arguments>
    inline void error(const std::string &message, Arguments... args)
    {
        fmt::fprintf(std::cerr, "ERROR: " + message + "\n", std::forward<Arguments>(args)...);
    }

    template<typename... Arguments>
    inline void fatal(const std::string &message, Arguments... args)
    {
        fmt::fprintf(std::cerr, "FATAL: " + message + "\n", std::forward<Arguments>(args)...);
    }

    template<typename... Arguments>
    inline std::string format(const std::string &message, Arguments... args)
    {
        return fmt::sprintf(message, std::forward<Arguments>(args)...);
    }
};

// initialize instance
auto logger_base::_self = std::make_unique<logger_base>();


// global logging functions
// use like: LOG_WARNING("hello {}", "world");

template<typename... Arguments>
inline void LOG(Arguments... args)
{
    logger_base::self()->general(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline void LOG_INFO(Arguments... args)
{
    logger_base::self()->info(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline void LOG_WARNING(Arguments... args)
{
    logger_base::self()->warning(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline void LOG_ERROR(Arguments... args)
{
    logger_base::self()->error(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline void LOG_FATAL(Arguments... args)
{
    logger_base::self()->fatal(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline std::string format(Arguments... args)
{
    return logger_base::self()->format(std::forward<Arguments>(args)...);
}

#endif // LOGGER_LOG_HPP
