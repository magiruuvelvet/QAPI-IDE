#include "server.hpp"

#include <logger/log.hpp>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_ZLIB_SUPPORT
#include <httplib.h>

UnitTestServer::UnitTestServer(const std::uint16_t &port)
{
    this->server = std::make_shared<httplib::Server>();

    using namespace httplib;

    this->server->Get("/", [](const Request &, Response &res) {
        res.set_content("test response", "text/html; charset=utf-8");
        res.status = 200;
    });

    this->server->Get("/redir1", [](const Request &, Response &res) {
        res.set_redirect("/redir2");
        res.status = 301;
    });

    this->server->Get("/redir2", [](const Request &, Response &res) {
        res.set_redirect("/test");
        res.status = 301;
    });

    this->server->Get("/test", [](const Request &, Response &res) {
        res.set_content("test response 2", "text/html; charset=utf-8");
        res.status = 200;
    });

    // cpp-httplib server is blocking execution so we need
    // to detach it in its own independent thread to keep
    // the unit tests running
    this->server_thr = std::make_shared<std::thread>([&]{
        this->server->listen("127.0.0.1", port);
    });

    this->server_thr->detach();
}

UnitTestServer::~UnitTestServer()
{
    if (this->server)
    {
        this->server->stop();
    }
}

bool UnitTestServer::isRunning() const
{
    std::lock_guard lg{this->mutex};

    if (this->server)
    {
        return this->server->is_running();
    }

    return false;
}
