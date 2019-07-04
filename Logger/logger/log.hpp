#ifndef LOGGER_LOG_HPP
#define LOGGER_LOG_HPP

#include <iostream>
#include <memory>

#include <fmt/format.h>
#include <fmt/printf.h>

// common data types
#include <logger/support/map.hpp>

// basic logger

struct logger_base final
{
public:
    static inline auto *self()
    { return _self.get(); }

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
    friend std::string format(Arguments... args);

private:
    static std::unique_ptr<logger_base> _self;

    enum class log_channel {
        NONE,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        TODO,
    };

    // TODO: support for files and optimal terminal formatting

    template<typename... Arguments>
    inline void general(const std::string &message, Arguments... args)
    {
        std::fprintf(stdout, "%s\n", fmt::format(message, std::forward<Arguments>(args)...).c_str());
    }

    template<typename... Arguments>
    inline void info(const std::string &message, Arguments... args)
    {
        std::fprintf(stdout, "%s\n", fmt::format("INFO: " + message, std::forward<Arguments>(args)...).c_str());
    }

    template<typename... Arguments>
    inline void warning(const std::string &message, Arguments... args)
    {
        std::fprintf(stderr, "%s\n", fmt::format("WARNING: " + message, std::forward<Arguments>(args)...).c_str());
    }

    template<typename... Arguments>
    inline void error(const std::string &message, Arguments... args)
    {
        std::fprintf(stderr, "%s\n", fmt::format("ERROR: " + message, std::forward<Arguments>(args)...).c_str());
    }

    template<typename... Arguments>
    inline void fatal(const std::string &message, Arguments... args)
    {
        std::fprintf(stderr, "%s\n", fmt::format("FATAL: " + message, std::forward<Arguments>(args)...).c_str());
    }

    template<typename... Arguments>
    inline void todo(const std::string &message, Arguments... args)
    {
        std::fprintf(stdout, "%s\n", fmt::format("TODO: " + message, std::forward<Arguments>(args)...).c_str());
    }

    template<typename... Arguments>
    inline std::string format(const std::string &message, Arguments... args)
    {
        return fmt::format(message, std::forward<Arguments>(args)...);
    }
};


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
inline void LOG_TODO(Arguments... args)
{
    logger_base::self()->todo(std::forward<Arguments>(args)...);
}

template<typename... Arguments>
inline std::string format(Arguments... args)
{
    return logger_base::self()->format(std::forward<Arguments>(args)...);
}

#endif // LOGGER_LOG_HPP
