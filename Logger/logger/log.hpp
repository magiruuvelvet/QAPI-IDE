#ifndef LOGGER_LOG_HPP
#define LOGGER_LOG_HPP

#include <cstdio>
#include <string>
#include <string_view>
#include <map>

#include <mutex>

#include <fmt/format.h>
#include <fmt/printf.h>

#include <logger/version.hpp>

// common data types
#include <logger/support/map.hpp>

// TODO: support for files and optimal terminal formatting

// basic logger
struct logger_base
{
    friend struct logger;

private:
    logger_base() = delete;

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
    constexpr inline static void print_##func_name(const std::string &message, Arguments... args) {          \
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

    // thread safety for printing to console
    static std::mutex log_print_mutex;

#undef DECLARE_LOG_CHANNEL
#undef IMPLEMENT_LOG_CHANNEL_PRINT
};

struct logger final : private logger_base
{
public:
    static constexpr const auto &system_name() { return logger_os_details::system_name; }
    static constexpr const auto &system_version() { return logger_os_details::system_version; }

    // returns the operating system name and version as string
    inline static const std::string get_system_details()
    {
        return logger_base::format("{} {}", system_name(), system_version());
    }

private:
    logger() = delete;
};


// global logging functions
// use like: LOG_WARNING("hello {}", "world");

template<typename... Arguments>
void LOG(Arguments... args)
{{
    std::lock_guard lock{logger_base::log_print_mutex};
    logger_base::print_general(std::forward<Arguments>(args)...);
}}

template<typename... Arguments>
void LOG_INFO(Arguments... args)
{{
    std::lock_guard lock{logger_base::log_print_mutex};
    logger_base::print_info(std::forward<Arguments>(args)...);
}}

template<typename... Arguments>
void LOG_WARNING(Arguments... args)
{{
    std::lock_guard lock{logger_base::log_print_mutex};
    logger_base::print_warning(std::forward<Arguments>(args)...);
}}

template<typename... Arguments>
void LOG_ERROR(Arguments... args)
{{
    std::lock_guard lock{logger_base::log_print_mutex};
    logger_base::print_error(std::forward<Arguments>(args)...);
}}

template<typename... Arguments>
void LOG_FATAL(Arguments... args)
{{
    std::lock_guard lock{logger_base::log_print_mutex};
    logger_base::print_fatal(std::forward<Arguments>(args)...);
}}

template<typename... Arguments>
void LOG_TODO(Arguments... args)
{{
    std::lock_guard lock{logger_base::log_print_mutex};
    logger_base::print_todo(std::forward<Arguments>(args)...);
}}

template<typename... Arguments>
inline const std::string format(Arguments... args)
{
    return logger_base::format(std::forward<Arguments>(args)...);
}

#endif // LOGGER_LOG_HPP
