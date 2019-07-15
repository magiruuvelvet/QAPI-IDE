#include <logger/log.hpp>

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("Logger", []{
        it("[file]", [&]{
            AssertThat(std::string{UNIT_TEST_TEMPORARY_DIR}.size(), IsGreaterThan(0));

            // write sample file
            std::ofstream of(std::string{UNIT_TEST_TEMPORARY_DIR} + "/file.test");
            AssertThat(of.is_open(), Equals(true));
            LOG_INFO(&of, "hello {}", "world");
            of.flush();

            // compare contents with excepted result
            std::ifstream if_(std::string{UNIT_TEST_TEMPORARY_DIR} + "/file.test");
            std::string res((std::istreambuf_iterator<char>(if_)), std::istreambuf_iterator<char>());
            AssertThat(res, Equals("[INFO] hello world\n"));
        });
    });
});
