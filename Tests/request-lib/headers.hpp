#include <requestlib/request.hpp>

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("RequestLib", []{
        it("[hasHeader]", [&]{
            Request req("");
            AssertThat(req.hasHeader("Test"), Equals(false));
            req.setHeader("Test", "");
            AssertThat(req.hasHeader("Test"), Equals(true));
        });

        it("[removeHeader]", [&]{
            Request req("");
            req.setHeader("Test", "");
            req.removeHeader("Test");
            AssertThat(req.hasHeader("Test"), Equals(false));
        });
    });
});
