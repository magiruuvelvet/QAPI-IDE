#include <iostream>

#define UNIT_TESTING

#include <logger/log.hpp>

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

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

int main(int argc, char **argv)
{
    named_thread::set_name(std::this_thread::get_id(), "TestRunner");

    LOG("Running tests on {}...", logger::get_system_details());

    UnitTestServer server;

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

    return bandit::run(argc, argv);
}
