#include <requestlib/request.hpp>
#include <fmt/format.h>
#include "server.hpp"

#include <bandit/bandit.h>

#include <benchmark.hpp>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("RequestLib", []{

        /**
         * Performs a simple HTTP request using an insecure connection.
         */
        benchmark_it("[basic-request HTTP]", [&]{
            Request req(fmt::format("http://127.0.0.1:{}", SERVER_PORT));
            const auto res = req.performRequest();
            AssertThat(res.successful(), Equals(true));
            AssertThat(res.status(), Equals(200));
            AssertThat(res.hasHeader("Content-Type"), Equals(true));
            AssertThat(res.getHeaderValue("Content-Type"), Equals("text/html; charset=utf-8"));
            AssertThat(res.body(), Equals("test response"));
        });

        /**
         * Performs a simple HTTP request using a secure TLS connection.
         */
        benchmark_it("[basic-request HTTPS]", [&]{
            Request req(fmt::format("https://127.0.0.1:{}", SERVER_PORT_SSL));
            req.verifyCertificate(false);
            const auto res = req.performRequest();
            AssertThat(res.successful(), Equals(true));
            AssertThat(res.status(), Equals(200));
            AssertThat(res.hasHeader("Content-Type"), Equals(true));
            AssertThat(res.getHeaderValue("Content-Type"), Equals("text/html; charset=utf-8"));
            AssertThat(res.body(), Equals("test response"));
        });
    });
});
