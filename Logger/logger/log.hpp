#ifndef LOGGER_LOG_HPP
#define LOGGER_LOG_HPP

/**
 * Global logging functions
 *
 * Formatting powered by fmt (https://github.com/fmtlib/fmt)
 *
 * --> Print formatted message to terminal (thread safe and synchronized)
 * LOG_channel("your formatted message", args...);
 *
 * --> Write formatted message to a std::ostream object (not thread safe, use one stream per thread or a mutex)
 * LOG_channel(&stringstream, "your formatted message", args...);
 *
 * --> Write formatted message to a std::ofstream file (not thread safe, use one stream per thread or a mutex, not flushed automatically)
 *     Call .flush() on the file stream as needed yourself.
 * LOG_channel(&fstream, "your formatted message", args...);
 *
 *
 * All logging functions append a new line (\n) character automatically.
 *
 *
 * The logging toggle function is not thread-safe!
 *
 *
 * For general string formatting use the format() function with the same
 * parameters as the terminal printing one.
 *
 * std::string result = format("your formatted string", args...);
 *
 */

#include <cstdio>
#include <string>
#include <string_view>
#include <ostream>
#include <fstream>

#include <frozen/unordered_map.h>

#include <mutex>

#include <fmt/format.h>
#include <fmt/printf.h>

#include <logger/named_thread.hpp>
#include <logger/version.hpp>

// common data types
#include <logger/support/map.hpp>

// TODO: support for optimal terminal formatting

// basic logger
struct logger_base
{
    friend struct logger;

private:
    logger_base() = delete;

    template<typename... Arguments> friend void LOG(const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_INFO(const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_WARNING(const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_ERROR(const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_FATAL(const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_TODO(const std::string &message, Arguments... args);

    template<typename... Arguments> friend void LOG(std::ostream *stream, const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_INFO(std::ostream *stream, const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_WARNING(std::ostream *stream, const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_ERROR(std::ostream *stream, const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_FATAL(std::ostream *stream, const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_TODO(std::ostream *stream, const std::string &message, Arguments... args);

    friend constexpr void LOG(bool enabled);
    friend constexpr void LOG_INFO(bool enabled);
    friend constexpr void LOG_WARNING(bool enabled);
    friend constexpr void LOG_ERROR(bool enabled);
    friend constexpr void LOG_FATAL(bool enabled);
    friend constexpr void LOG_TODO(bool enabled);

    template<typename... Arguments> friend const std::string format(const std::string &message, Arguments... args);

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
        static bool enabled;                                               \
    }

    template<> struct log_channel_config<log_channel::NONE>
    {
        static const fp_t fp;
        static constexpr const std::string_view name = "";
        static constexpr const log_channel channel = log_channel::NONE;
        static bool enabled;
    };

    static constexpr bool enabled(log_channel c)
    {
        switch (c)
        {
            case log_channel::NONE:     return log_channel_config<log_channel::NONE>::enabled;
            case log_channel::INFO:     return log_channel_config<log_channel::INFO>::enabled;
            case log_channel::WARNING:  return log_channel_config<log_channel::WARNING>::enabled;
            case log_channel::ERROR:    return log_channel_config<log_channel::ERROR>::enabled;
            case log_channel::FATAL:    return log_channel_config<log_channel::FATAL>::enabled;
            case log_channel::TODO:     return log_channel_config<log_channel::TODO>::enabled;
        }
    }

    static constexpr void setEnabled(log_channel c, bool enabled)
    {
        switch (c)
        {
            case log_channel::NONE:     log_channel_config<log_channel::NONE>::enabled = enabled; break;
            case log_channel::INFO:     log_channel_config<log_channel::INFO>::enabled = enabled; break;
            case log_channel::WARNING:  log_channel_config<log_channel::WARNING>::enabled = enabled; break;
            case log_channel::ERROR:    log_channel_config<log_channel::ERROR>::enabled = enabled; break;
            case log_channel::FATAL:    log_channel_config<log_channel::FATAL>::enabled = enabled; break;
            case log_channel::TODO:     log_channel_config<log_channel::TODO>::enabled = enabled; break;
        }
    }

    DECLARE_LOG_CHANNEL(INFO);
    DECLARE_LOG_CHANNEL(WARNING);
    DECLARE_LOG_CHANNEL(ERROR);
    DECLARE_LOG_CHANNEL(FATAL);
    DECLARE_LOG_CHANNEL(TODO);

#undef DECLARE_LOG_CHANNEL

    static constexpr const frozen::unordered_map<log_channel, std::string_view, 6> log_channels = {
        {log_channel::NONE,     log_channel_config<log_channel::NONE>::name},
        {log_channel::INFO,     log_channel_config<log_channel::INFO>::name},
        {log_channel::WARNING,  log_channel_config<log_channel::WARNING>::name},
        {log_channel::ERROR,    log_channel_config<log_channel::ERROR>::name},
        {log_channel::FATAL,    log_channel_config<log_channel::FATAL>::name},
        {log_channel::TODO,     log_channel_config<log_channel::TODO>::name},
    };

    static const std::string fmt_log_channel(log_channel channel, const std::string_view &fmt);
    static constexpr const std::string_view print_fmt = "[{}] {}: ";
    static constexpr const std::string_view stream_fmt = "[{}::{}] ";

    // default terminal printing of log messages
    // channel format: "CHANNEL: message"
    // none format:    "message"
    template<typename... Arguments>
    constexpr inline static void print(const std::string &message, log_channel channel, fp_t fp, Arguments... args)
    {
        std::fprintf(fp, "%s\n", (fmt_log_channel(channel, print_fmt) + fmt::format(message, std::forward<Arguments>(args)...)).c_str());
    }

    // writes log messages to any std::ostream compatible object (like stringstream or ofstream)
    // all messages are ended with a new line automatically
    // stream needs to be flushed manually as needed
    // channel format: "[CHANNEL] message"
    // none format: "message"
    template<typename... Arguments>
    constexpr inline static void stream(const std::string &message, log_channel channel, std::ostream *stream, Arguments... args)
    {
        (*stream) << (fmt_log_channel(channel, stream_fmt) + fmt::format(message, std::forward<Arguments>(args)...)) << '\n';
    }

    template<typename... Arguments>
    inline static const std::string format(const std::string &message, Arguments... args)
    {
        return fmt::format(message, std::forward<Arguments>(args)...);
    }

    // thread safety for printing to console
    static std::mutex log_print_mutex;
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

// note for const char* overload:
//   C++ prefers bool over std::string for "const char*" string literals
//   when looking through all function overloads, hence this overload
//   exists

#define IMPLEMENT_LOG_CHANNEL(func_name, channel)                                         \
    template<typename... Arguments> __attribute__((noinline))                             \
    void func_name(const std::string &message, Arguments... args) {                       \
        using lb = logger_base;                                                           \
        if (!lb::enabled(lb::log_channel::channel)) { return; }                           \
        std::lock_guard lock{logger_base::log_print_mutex};                               \
        lb::print(message,                                                                \
                  lb::log_channel::channel,                                               \
                  lb::log_channel_config<lb::log_channel::channel>::fp,                   \
                  std::forward<Arguments>(args)...);                                      \
    }                                                                                     \
    template<typename... Arguments>                                                       \
    inline constexpr void func_name(const char *message, Arguments... args) {             \
        func_name(std::string{message}, std::forward<Arguments>(args)...);                \
    }                                                                                     \
    template<typename... Arguments> __attribute__((noinline))                             \
    void func_name(std::ostream *stream, const std::string &message,                      \
                          Arguments... args) {                                            \
        using lb = logger_base;                                                           \
        if (!lb::enabled(lb::log_channel::channel)) { return; }                           \
        lb::stream(message, lb::log_channel::channel, stream,                             \
                   std::forward<Arguments>(args)...);                                     \
    }                                                                                     \
    inline constexpr void func_name(bool enabled) {                                       \
        logger_base::setEnabled(logger_base::log_channel::channel, enabled);              \
    }

IMPLEMENT_LOG_CHANNEL(LOG,          NONE);
IMPLEMENT_LOG_CHANNEL(LOG_INFO,     INFO);
IMPLEMENT_LOG_CHANNEL(LOG_WARNING,  WARNING);
IMPLEMENT_LOG_CHANNEL(LOG_ERROR,    ERROR);
IMPLEMENT_LOG_CHANNEL(LOG_FATAL,    FATAL);
IMPLEMENT_LOG_CHANNEL(LOG_TODO,     TODO);

template<typename... Arguments>
inline const std::string format(const std::string &message, Arguments... args)
{
    return logger_base::format(message, std::forward<Arguments>(args)...);
}

#endif // LOGGER_LOG_HPP
