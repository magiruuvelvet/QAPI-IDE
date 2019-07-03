#include <logger/log.hpp>

// initialize instance
auto logger_base::_self = std::make_unique<logger_base>();
