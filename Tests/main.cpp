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
    LOG("Running tests on {}...", logger::get_system_details());

    UnitTestServer server(SERVER_PORT);

    while (!server.isRunning())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return bandit::run(argc, argv);
}
