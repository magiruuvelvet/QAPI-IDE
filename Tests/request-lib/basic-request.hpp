#include <requestlib/request.hpp>

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("RequestLib", []{
        it("[basic-request HTTP]", [&]{
            Request req("http://127.0.0.1");
            const auto res = req.performRequest();
            AssertThat(res.successful(), Equals(true));
            AssertThat(res.status(), Equals(200));
        });

        it("[basic-request HTTPS]", [&]{
            Request req("https://localhost");
            const auto res = req.performRequest();
            AssertThat(res.successful(), Equals(true));
            AssertThat(res.status(), Equals(200));
        });
    });
});
