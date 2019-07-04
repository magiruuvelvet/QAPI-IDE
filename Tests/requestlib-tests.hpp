#include <requestlib/request.hpp>

#include <logger/log.hpp>
#include <logger_support/request.hpp>
#include <logger_support/response.hpp>

void requestlib_tests()
{
    Request req("http://127.0.0.1");
    req.setHeader("X-Test", "test");
    LOG("Test Request: {}", req);
    auto res = req.performRequest();
    LOG("Test Response: {}", res);

    Request req2("https://localhost");
    res = req2.performRequest();
    LOG("Test Response: {}", res);
}
