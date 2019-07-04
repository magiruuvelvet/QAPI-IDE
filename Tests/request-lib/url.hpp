#include <requestlib/request.hpp>

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

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
    });
});
