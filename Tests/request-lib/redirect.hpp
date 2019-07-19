#include <requestlib/request.hpp>
#include <fmt/format.h>
#include "server.hpp"

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("RequestLib", []{

        /**
         * Test recursive request redirection.
         *  /redir1 -> /redir2
         *  /redir2 -> /test
         * 2 redirects in total.
         *
         * Final status code is expected to be 200 with a response body.
         */
        it("[redirect with follow]", [&]{
            Request req(fmt::format("http://127.0.0.1:{}/redir1", SERVER_PORT));
            req.followRedirects(true);
            const auto res = req.performRequest();
            AssertThat(res.successful(), Equals(true));
            AssertThat(res.status(), Equals(200));
            AssertThat(res.hasHeader("Content-Type"), Equals(true));
            AssertThat(res.body(), Equals("test response 2"));
        });

        /**
         * Test request to redirection resource without redirecting.
         * Status code is expected to be 301.
         */
        it("[redirect without follow]", [&]{
            Request req(fmt::format("http://127.0.0.1:{}/redir1", SERVER_PORT));
            const auto res = req.performRequest();
            AssertThat(res.successful(), Equals(true));
            AssertThat(res.status(), Equals(301));
            AssertThat(res.hasHeader("Location"), Equals(true));
        });
    });
});
