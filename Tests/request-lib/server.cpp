#include "server.hpp"

#include <logger/log.hpp>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_ZLIB_SUPPORT
#include <httplib.h>

UnitTestServer::UnitTestServer()
{
    this->server = std::make_shared<httplib::Server>();
    this->server_ssl = std::make_shared<httplib::SSLServer>(
        (std::string{UNIT_TEST_REQUESTLIB_CERT_DIR} + "/test.cert").c_str(),
        (std::string{UNIT_TEST_REQUESTLIB_CERT_DIR} + "/test.key").c_str());

    using namespace httplib;

    this->server->Get("/", [](const Request &, Response &res) {
        res.set_content("test response", "text/html; charset=utf-8");
        res.status = 200;
    });

    this->server_ssl->Get("/", [](const Request &, Response &res) {
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
    this->server_thr = std::make_shared<named_thread>("HttpServer", [&]{
        LOG("Starting HTTP server on port {}...", SERVER_PORT);
        this->server->listen("127.0.0.1", SERVER_PORT);
    });
    this->server_thr->detach();

    this->server_ssl_thr = std::make_shared<named_thread>("HttpsServer", [&]{
        LOG("Starting HTTPS server on port {}...", SERVER_PORT_SSL);
        this->server_ssl->listen("127.0.0.1", SERVER_PORT_SSL);
    });
    this->server_ssl_thr->detach();
}

UnitTestServer::~UnitTestServer()
{
    if (this->server)
    {
        this->server->stop();
    }
    if (this->server_ssl)
    {
        this->server_ssl->stop();
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

bool UnitTestServer::isSSLRunning() const
{
    std::lock_guard lg{this->mutex_ssl};

    if (this->server_ssl)
    {
        return this->server_ssl->is_running();
    }

    return false;
}
