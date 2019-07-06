#include <iostream>

#define UNIT_TESTING

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

// Logger tests
#include "logger/fmt-test.hpp"

// RequestLib tests
#include "request-lib/basic-request.hpp"
#include "request-lib/redirect.hpp"
#include "request-lib/url.hpp"
#include "request-lib/headers.hpp"

int main(int argc, char **argv)
{
    return bandit::run(argc, argv);
}
