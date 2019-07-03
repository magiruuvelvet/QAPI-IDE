#include <requestlib/request.hpp>

void requestlib_tests()
{
    Request req("http://127.0.0.1");
    auto res = req.performRequest();
    std::cout << "Response: " << res.status() << std::endl;

    Request req2("https://localhost");
    res = req2.performRequest();
    std::cout << "Response: " << res.status() << std::endl;
}
