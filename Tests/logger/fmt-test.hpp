#include <logger/log.hpp>

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("Logger", []{
        it("[fmt]", [&]{
            const auto res = format("hello {} {} {:.2f} {}", "world", 3, 3.3, 3);
            AssertThat(res, Equals("hello world 3 3.30 3"));
        });
    });
});
