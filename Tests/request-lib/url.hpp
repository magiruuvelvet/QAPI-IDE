#include <requestlib/request.hpp>

#include <bandit/bandit.h>
//#include <bandit/assertion_exception.h>

using namespace snowhouse;
using namespace bandit;

#include <logger/log.hpp>
#include <logger_support/request.hpp>

go_bandit([]{
    describe("RequestLib", []{
        it("[URL 1]", [&]{
            Request req("http://127.0.0.1");
            AssertThat(req.url(), Equals("http://127.0.0.1:80/"));
        });

        it("[URL 2]", [&]{
            Request req("https://localhost");
            AssertThat(req.url(), Equals("https://localhost:443/"));
        });

        it("[URL invalid]", [&]{
            // validate that the correct exception is thrown on URL parse error
            AssertThrows(UrlParseException, Request("http://localhost:abc/path"));
        });
    });
});
