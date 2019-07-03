#include <iostream>
#include <cstdlib>

#include "logger-test.hpp"
#include "requestlib-tests.hpp"

// TODO: write real unit tests with assertions

int main()
{
    logger_test();
    requestlib_tests();

    return 0;
}
