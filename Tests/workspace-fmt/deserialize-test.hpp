#include <workspace/workspace.hpp>

#include <bandit/bandit.h>

#include <benchmark.hpp>

#include <algorithm>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("WorkspaceFormat", []{

        constexpr const std::string_view sample_workspace_json = R"(
{
    "name": "test workspace",
    "requestgroups": [
        {
            "name": "group 1",
            "requests": [
                {
                    "headers": {},
                    "name": "req 1",
                    "url": "http://localhost"
                }
            ]
        },
        {
            "name": "group 2",
            "requests": [
                {
                    "headers": {
                        "Content-Type": ["application/json"],
                        "Set-Cookie": ["some cookie", "another cookie"]
                    },
                    "name": "req 1",
                    "url": "http://127.0.0.1/v1/api"
                },
                {
                    "headers": {},
                    "name": "req 2",
                    "url": "http://127.0.0.1/v2/api"
                }
            ]
        }
    ]
}
        )";

        benchmark_it("[deserialize workspace]", [&]{
            bool success = false;
            Workspace workspace = Workspace::deserialize(std::string{sample_workspace_json}, &success);

            AssertThat(success, Equals(true));
            AssertThat(workspace.name(), Equals("test workspace"));

            const auto &groups = workspace.requestGroups();
            AssertThat(groups.size(), Equals(2));

            const auto &group1 = groups.front();
            AssertThat(group1.name(), Equals("group 1"));
            AssertThat(group1.requests().size(), Equals(1));

            AssertThat(group1.requests().front().headers().size(), Equals(0));
            AssertThat(group1.requests().front().name(), Equals("req 1"));
            AssertThat(group1.requests().front().url(), Equals("http://localhost"));

            const auto &group2 = groups.back();
            AssertThat(group2.name(), Equals("group 2"));
            AssertThat(group2.requests().size(), Equals(2));

            AssertThat(group2.requests().front().headers().size(), Equals(3));
            AssertThat(group2.requests().front().name(), Equals("req 1"));
            AssertThat(group2.requests().front().url(), Equals("http://127.0.0.1/v1/api"));

            AssertThat(group2.requests().front().hasHeader("Content-Type"), Equals(true));
            const auto &contentType = group2.requests().front().headerValues("Content-Type");
            AssertThat(std::count(contentType.begin(), contentType.end(), "application/json"), Equals(1));

            AssertThat(group2.requests().front().hasHeader("Set-Cookie"), Equals(true));
            const auto &cookies = group2.requests().front().headerValues("Set-Cookie");
            AssertThat(std::count(cookies.begin(), cookies.end(), "some cookie"), Equals(1));
            AssertThat(std::count(cookies.begin(), cookies.end(), "another cookie"), Equals(1));

            AssertThat(group2.requests().back().headers().size(), Equals(0));
            AssertThat(group2.requests().back().name(), Equals("req 2"));
            AssertThat(group2.requests().back().url(), Equals("http://127.0.0.1/v2/api"));
        });
    });
});
