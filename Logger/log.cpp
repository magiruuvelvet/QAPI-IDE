#include <logger/log.hpp>

std::mutex logger_base::log_print_mutex;

const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::NONE>::fp =     stdout;
const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::INFO>::fp =     stdout;
const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::WARNING>::fp =  stderr;
const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::ERROR>::fp =    stderr;
const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::FATAL>::fp =    stderr;
const logger_base::fp_t logger_base::log_channel_config<logger_base::log_channel::TODO>::fp =     stdout;

bool logger_base::log_channel_config<logger_base::log_channel::NONE>::enabled =     true;
bool logger_base::log_channel_config<logger_base::log_channel::INFO>::enabled =     true;
bool logger_base::log_channel_config<logger_base::log_channel::WARNING>::enabled =  true;
bool logger_base::log_channel_config<logger_base::log_channel::ERROR>::enabled =    true;
bool logger_base::log_channel_config<logger_base::log_channel::FATAL>::enabled =    true;
bool logger_base::log_channel_config<logger_base::log_channel::TODO>::enabled =     true;

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
