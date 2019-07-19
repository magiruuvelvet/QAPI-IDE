#ifndef TESTS_SERVER_HPP
#define TESTS_SERVER_HPP

#include <memory>
#include <thread>
#include <mutex>

static constexpr const std::uint16_t SERVER_PORT = 9347;

namespace httplib {
    class Server;
}

class UnitTestServer final
{
public:
    UnitTestServer(const std::uint16_t &port);
    ~UnitTestServer();

    bool isRunning() const;

private:
    std::shared_ptr<httplib::Server> server;
    std::shared_ptr<std::thread> server_thr;
    mutable std::mutex mutex;
};

#endif // TESTS_SERVER_HPP
