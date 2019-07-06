#include <requestlib/request.hpp>

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("RequestLib", []{

        // FIXME: this test only works on my machine
        //  first redirect is HTTP to HTTPS with absolute URL
        //  second redirect is a relative path redirect to /ja/homepage
        //  final status code is 200 with 2 redirects
        it("[redirect with follow]", [&]{
            Request req("http://kimai.local");
            req.followRedirects(true);
            const auto res = req.performRequest();
            AssertThat(res.successful(), Equals(true));
            AssertThat(res.status(), Equals(200));
            AssertThat(res.hasHeader("Content-Type"), Equals(true));
        });

        it("[redirect without follow]", [&]{
            Request req("http://kimai.local");
            const auto res = req.performRequest();
            AssertThat(res.successful(), Equals(true));
            AssertThat(res.status(), Equals(301));
            AssertThat(res.hasHeader("Content-Type"), Equals(true));
        });
    });
});
