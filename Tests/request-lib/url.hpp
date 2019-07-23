#include <requestlib/request.hpp>

#include <bandit/bandit.h>

#include <benchmark.hpp>

using namespace snowhouse;
using namespace bandit;

/**
 * Several URL parsing and normalization tests.
 */

go_bandit([]{
    describe("RequestLib", []{

        /**
         * HTTP protocol input without explicit port overwrite results in
         * default port of 80.
         */
        benchmark_it("[URL HTTP default port]", [&]{
            Request req("http://127.0.0.1");
            AssertThat(req.url(), Equals("http://127.0.0.1:80/"));
        });

        /**
         * HTTPS protocol input without explicit port overwrite results in
         * default port of 443.
         */
        benchmark_it("[URL HTTPS default port]", [&]{
            Request req("https://localhost");
            AssertThat(req.url(), Equals("https://localhost:443/"));
        });

        /**
         * HTTP protocol input with port overwrite should result
         * in the given port instead of the default one.
         */
        benchmark_it("[URL HTTP overwritten port]", [&]{
            Request req("http://127.0.0.1:8080");
            AssertThat(req.url(), Equals("http://127.0.0.1:8080/"));
        });

        /**
         * HTTPS protocol input with port overwrite should result
         * in the given port instead of the default one.
         */
        benchmark_it("[URL HTTPS overwritten port]", [&]{
            Request req("https://127.0.0.1:8443");
            AssertThat(req.url(), Equals("https://127.0.0.1:8443/"));
        });

        /**
         * Validate that the correct exception is thrown on URL parse error.
         * Here we have an invalid port number which are letters.
         */
        benchmark_it("[URL invalid]", [&]{
            AssertThrows(UrlParseException, Request("http://localhost:abc/path"));
        });

        /**
         * Validate that empty input isn't touched and results in an empty string.
         */
        benchmark_it("[URL empty]", [&]{
            Request req("");
            AssertThat(req.url(), Equals(""));
        });
    });
});
