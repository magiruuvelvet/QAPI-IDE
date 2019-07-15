#include <iostream>

#define UNIT_TESTING

#include <logger/log.hpp>

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

// Logger tests
#include "logger/fmt-test.hpp"
#include "logger/file-test.hpp"

// RequestLib tests
#include "request-lib/basic-request.hpp"
#include "request-lib/redirect.hpp"
#include "request-lib/url.hpp"
#include "request-lib/headers.hpp"

// ScriptingInterface tests
#include "script/cpp.hpp"

int main(int argc, char **argv)
{
    LOG("Running tests on {}...", logger::get_system_details());
    return bandit::run(argc, argv);
}
