#include <logger/log.hpp>

const std::map<logger_base::log_channel, std::string> logger_base::log_channels = {
    {logger_base::log_channel::NONE,     ""},
    {logger_base::log_channel::INFO,     "INFO"},
    {logger_base::log_channel::WARNING,  "WARNING"},
    {logger_base::log_channel::ERROR,    "ERROR"},
    {logger_base::log_channel::FATAL,    "FATAL"},
    {logger_base::log_channel::TODO,     "TODO"},
};

const std::string logger_base::fmt_log_channel(log_channel channel, const std::string &fmt)
{
    if (log_channels.at(channel).empty())
    {
        return {};
    }
    else
    {
        return fmt::format(fmt, log_channels.at(channel));
    }
}
