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
 * --> Write formatted message to a std::ostream object (not thread safe, use one stream per thread)
 * LOG_channel(&stringstream, "your formatted message", args...);
 *
 * --> Write formatted message to a std::ofstream file (thread safe and synchronized, not flushed automatically)
 *     Call .flush() on the file stream as needed yourself.
 * LOG_channel(&fstream, "your formatted message", args...);
 *
 *
 * All logging functions append a new line (\n) character automatically.
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
#include <unordered_map>

#include <mutex>

#include <fmt/format.h>
#include <fmt/printf.h>

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

    template<typename... Arguments> friend void LOG(std::ofstream *stream, const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_INFO(std::ofstream *stream, const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_WARNING(std::ofstream *stream, const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_ERROR(std::ofstream *stream, const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_FATAL(std::ofstream *stream, const std::string &message, Arguments... args);
    template<typename... Arguments> friend void LOG_TODO(std::ofstream *stream, const std::string &message, Arguments... args);

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

#undef DECLARE_LOG_CHANNEL

    // FIXME: this should be compile-time constexpr and not run time!
    static const std::unordered_map<const log_channel, const std::string_view> log_channels;

    static const std::string fmt_log_channel(log_channel channel, const std::string_view &fmt);
    static constexpr const std::string_view print_fmt = "{}: ";
    static constexpr const std::string_view stream_fmt = "[{}]: ";

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

    // thread safety for writing to file streams
    static std::mutex log_file_mutex;
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


#define IMPLEMENT_LOG_CHANNEL(func_name, channel)                                         \
    template<typename... Arguments> __attribute__((noinline))                             \
    void func_name(const std::string &message, Arguments... args) {                       \
        using lb = logger_base;                                                           \
        std::lock_guard lock{logger_base::log_print_mutex};                               \
        lb::print(message,                                                                \
                  lb::log_channel::channel,                                               \
                  lb::log_channel_config<lb::log_channel::channel>::fp,                   \
                  std::forward<Arguments>(args)...);                                      \
    }                                                                                     \
    template<typename... Arguments>                                                       \
    inline void func_name(std::ostream *stream, const std::string &message,               \
                          Arguments... args) {                                            \
        using lb = logger_base;                                                           \
        lb::stream(message, lb::log_channel::channel, stream,                             \
                   std::forward<Arguments>(args)...);                                     \
    }                                                                                     \
    template<typename... Arguments> __attribute__((noinline))                             \
    void func_name(std::ofstream *stream, const std::string &message,                     \
                          Arguments... args) {                                            \
        using lb = logger_base;                                                           \
        std::lock_guard lock{logger_base::log_file_mutex};                                \
        lb::stream(message, lb::log_channel::channel, stream,                             \
                   std::forward<Arguments>(args)...);                                     \
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
