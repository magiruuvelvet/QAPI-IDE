#include <logger/log.hpp>

#include <bandit/bandit.h>

#include <sstream>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("Logger", []{

        /**
         * Test if disabling and enabling of log channels works.
         */
        it("[channel toggle]", [&]{
            std::stringstream s;
            LOG_INFO(&s, "hello world");
            LOG_INFO(false);
            LOG_INFO(&s, "ignored");
            LOG_WARNING(&s, "warning");
            LOG_INFO(true);
            LOG_INFO(&s, "not ignored");

            AssertThat(s.str(), Equals(
R"([INFO] hello world
[WARNING] warning
[INFO] not ignored
)"
            ));
        });
    });
});
