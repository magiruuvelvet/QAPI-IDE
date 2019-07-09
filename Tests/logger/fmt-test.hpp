#include <logger/log.hpp>

#include <bandit/bandit.h>

#include <sstream>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("Logger", []{
        it("[fmt]", [&]{
            const auto res = format("hello {} {} {:.2f} {}", "world", 3, 3.3, 3);
            AssertThat(res, Equals("hello world 3 3.30 3"));
        });

        it("[stringstream 1]", [&]{
            std::stringstream ss;
            LOG(&ss, "123 {} 5", "test", 5);
            AssertThat(ss.str(), Equals("123 test 5\n"));
        });

        it("[stringstream 2]", [&]{
            std::stringstream ss;
            LOG_INFO(&ss, "123 {} 5", "test", 5);
            AssertThat(ss.str(), Equals("[INFO]: 123 test 5\n"));
        });

        // booleans should be formatted as such, and not as numbers (1, 0)
        // if this test fails on your machine your compiler might have a bug
        // bug reports should go to https://github.com/fmtlib/fmt in this case
        it("[fmt bool]", [&]{
            const auto res = format("{} {}", true, false);
            AssertThat(res, Equals("true false"));
        });

        // test hexadecimal formatting
        it("[fmt hex]", [&]{
            const auto res = format("0x{:X}", 0xFF);
            AssertThat(res, Equals("0xFF"));
        });
    });
});
