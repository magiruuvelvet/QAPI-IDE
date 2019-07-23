#include <iostream>

#include <logger/log.hpp>

#include <bandit/bandit.h>

#include "benchmark.hpp"

// Logger tests
#include "logger/fmt-test.hpp"
#include "logger/file-test.hpp"
#include "logger/toggle-test.hpp"

// RequestLib tests
#include "request-lib/server.hpp"
#include "request-lib/basic-request.hpp"
#include "request-lib/redirect.hpp"
#include "request-lib/url.hpp"
#include "request-lib/headers.hpp"

// ScriptingInterface tests
#ifdef HAS_SCRIPTING_INTERFACE
#include "script/cpp.hpp"
#include "script/js.hpp"
#endif

bool check_has_info_reporter(const std::vector<const char*> &args)
{
    bool has_flag = false;
    for (auto&& arg : args)
    {
        if (std::strcmp(arg, "--reporter=info") == 0)
        {
            has_flag = true;
        }
    }
    return has_flag;
}

int main(int argc, char **argv)
{
    named_thread::set_name(std::this_thread::get_id(), "TestRunner");

    LOG("Running tests on {}...", logger::get_system_details());

    UnitTestServer server;

    // wait for the unit test server to start
    auto max_loop_count = 0xFFFFu, current_loop = 0u;
    while (!(server.isRunning() && server.isSSLRunning()))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ++current_loop;
        if (current_loop > max_loop_count)
        {
            // if reached here the http tests may fail as the server
            // could not be started in time or crashed
            break;
        }
    }

    // check if the current bandit reporter is "info"
    const std::vector<const char*> args(argv, argc + argv);
    const bool has_info_reporter = check_has_info_reporter(args);
    auto *logger = dynamic_cast<bandit::detail::default_benchmark_logger*>(bandit::detail::benchmark::registered_logger());

    // if not, disable the benchmark console printing,
    // as it works best with the "info" reporter
    if (!has_info_reporter)
    {
        logger->enable_printing(false);
    }
    else
    {
        logger->set_precision(10);
    }

    // run all registered tests
    auto ret = bandit::run(argc, argv);

    // show total time
    LOG("Total time: {:.10f} ms", logger->total_time());

    return ret;
}
