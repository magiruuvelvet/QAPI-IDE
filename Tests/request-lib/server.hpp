#ifndef TESTS_SERVER_HPP
#define TESTS_SERVER_HPP

#include <memory>
#include <thread>
#include <mutex>

class named_thread;

static constexpr const std::uint16_t SERVER_PORT = 9347;
static constexpr const std::uint16_t SERVER_PORT_SSL = 9348;

namespace httplib {
    class Server;
    class SSLServer;
}

class UnitTestServer final
{
public:
    UnitTestServer();
    ~UnitTestServer();

    bool isRunning() const;
    bool isSSLRunning() const;

private:
    std::shared_ptr<httplib::Server> server;
    std::shared_ptr<httplib::SSLServer> server_ssl;
    std::shared_ptr<named_thread> server_thr;
    std::shared_ptr<named_thread> server_ssl_thr;
    mutable std::mutex mutex;
    mutable std::mutex mutex_ssl;
};

#endif // TESTS_SERVER_HPP
