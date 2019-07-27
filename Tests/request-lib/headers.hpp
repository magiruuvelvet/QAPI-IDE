#include <requestlib/request.hpp>

#include <bandit/bandit.h>

#include <benchmark.hpp>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("RequestLib", []{
        benchmark_it("[hasHeader]", [&]{
            Request req("");
            AssertThat(req.hasHeader("Test"), Equals(false));
            req.setHeader("Test", "");
            AssertThat(req.hasHeader("Test"), Equals(true));
        });

        benchmark_it("[hasHeader case insensitive]", [&]{
            Request req("");
            AssertThat(req.hasHeader("Test"), Equals(false));
            req.setHeader("Test", "");
            AssertThat(req.hasHeader("TEST"), Equals(true));
        });

        benchmark_it("[removeHeader]", [&]{
            Request req("");
            req.setHeader("Test", "");
            req.removeHeader("Test");
            AssertThat(req.hasHeader("Test"), Equals(false));
        });

        benchmark_it("[removeHeader case insensitive]", [&]{
            Request req("");
            req.setHeader("Test", "");
            req.removeHeader("TEST");
            AssertThat(req.hasHeader("Test"), Equals(false));
        });

        benchmark_it("[header values]", [&]{
            Request req("");
            req.setHeader("Set-Cookie", "cookie 1");
            req.setHeader("Set-Cookie", "cookie 2");
            AssertThat(req.getHeaderValues("Set-Cookie"), Equals(std::list<std::string>{"cookie 1", "cookie 2"}));
            req.removeHeader("Set-Cookie");
            AssertThat(req.hasHeader("set-cookie"), Equals(false));
        });
    });
});
