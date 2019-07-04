#include <logger/log.hpp>

void logger_test()
{
    // result: "hello world 3 3.30 3" with the proper prefix
    LOG("hello {} {} {:.2f} {}", "world", 3, 3.3, 3);
    LOG_INFO("hello {} {} {:.2f} {}", "world", 3, 3.3, 3);
    LOG_WARNING("hello {} {} {:.2f} {}", "world", 3, 3.3, 3);
    LOG_ERROR("hello {} {} {:.2f} {}", "world", 3, 3.3, 3);
    LOG_FATAL("hello {} {} {:.2f} {}", "world", 3, 3.3, 3);

    // test string formatting
    auto res = format("hello {} {} {:.2f} {}", "world", 3, 3.3, 3);
    LOG(res);
}
