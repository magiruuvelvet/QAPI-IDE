#include <logger/log.hpp>

const std::map<const logger_base::log_channel, const std::string_view> logger_base::log_channels = {
    {logger_base::log_channel::NONE,     log_channel_config<log_channel::NONE>::name},
    {logger_base::log_channel::INFO,     log_channel_config<log_channel::INFO>::name},
    {logger_base::log_channel::WARNING,  log_channel_config<log_channel::WARNING>::name},
    {logger_base::log_channel::ERROR,    log_channel_config<log_channel::ERROR>::name},
    {logger_base::log_channel::FATAL,    log_channel_config<log_channel::FATAL>::name},
    {logger_base::log_channel::TODO,     log_channel_config<log_channel::TODO>::name},
};

std::mutex logger_base::log_print_mutex;

const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::NONE>::fp =     stdout;
const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::INFO>::fp =     stdout;
const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::WARNING>::fp =  stderr;
const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::ERROR>::fp =    stderr;
const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::FATAL>::fp =    stderr;
const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::TODO>::fp =     stdout;

const std::string logger_base::fmt_log_channel(log_channel channel, const std::string_view &fmt)
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
