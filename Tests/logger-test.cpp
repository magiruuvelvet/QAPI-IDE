#include <logger/log.hpp>

void logger_test()
{
    // result: "hello world 3 3.30 3" with the proper prefix
    LOG("hello %s %i %.2f %i", "world", 3, 3.3, 3);
    LOG_INFO("hello %s %i %.2f %i", "world", 3, 3.3, 3);
    LOG_WARNING("hello %s %i %.2f %i", "world", 3, 3.3, 3);
    LOG_ERROR("hello %s %i %.2f %i", "world", 3, 3.3, 3);
    LOG_FATAL("hello %s %i %.2f %i", "world", 3, 3.3, 3);

    // test string formatting
    auto res = format("hello %s %i %.2f %i", "world", 3, 3.3, 3);
    LOG(res);
}
