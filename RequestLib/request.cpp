#include <requestlib/request.hpp>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_ZLIB_SUPPORT
#include <httplib.h>

#include <url.h>

#include <logger/log.hpp>
#include <logger_support/request.hpp>
#include <logger_support/url.hpp>

Request::Request(const std::string &url, RequestMethod method, const std::string &custom_method)
    : _full_url(url)
{
    // set request method as string
    this->_method = RequestMethodString(method, custom_method);

    // parse url
    try {
        this->_url = std::make_shared<Url::Url>(url);
    } catch (Url::UrlParseException &e) {
        LOG_ERROR("Request: Url::UrlParseException: {}", e.what());
        this->_url = nullptr;
        return;
    }

    // fixup some derps from the url parsing library
    if (this->_url->scheme().empty())
    {
        // assume http by default for local development
        this->_url->setScheme("http");
    }

    if (this->_url->scheme() == "http" && this->_url->port() == 0)
    {
        this->_url->setPort(80);
    }
    else if (this->_url->scheme() == "https" && this->_url->port() == 0)
    {
        this->_url->setPort(443);
    }

    if (this->_url->path().empty())
    {
        this->_url->setPath("/");
    }

#ifdef DEBUG_BUILD
    LOG_INFO("{}, {}", *this, *this->_url);
    LOG_INFO(
              "Scheme:     {}\n"
        "      Userinfo:   {}\n"
        "      Host:       {}\n"
        "      Port:       {}\n"
        "      Path:       {}\n"
        "      Params:     {}\n"
        "      Query:      {}\n"
        "      Fragment:   {}\n",
        this->_url->scheme(),
        this->_url->userinfo(),
        this->_url->host(),
        this->_url->port(),
        this->_url->path(),
        this->_url->params(),
        this->_url->query(),
        this->_url->fragment(),
        this->_url->str()
    );
#endif
}

Request::~Request()
{
}

const std::string Request::url() const
{
    return this->_url->str();
}

void Request::setRequestBody(const std::string &data)
{
    this->_data = data;
}

void Request::setHeader(const std::string &header, const std::string &value)
{
    this->_headers.insert(std::pair<std::string, std::string>(header, value));
}

Response Request::performRequest()
{
    struct ClientWrapper {
        ClientWrapper(const Url::Url *url)
            : url(url)
        {
        }

        // wrapper around Client and SSLClient to support both at the same time
        bool send(httplib::Request &req, httplib::Response &res)
        {
            if (this->url->scheme() == "https")
            {
                httplib::SSLClient client(this->url->host().c_str(), this->url->port());
                // TODO: certificate verify support toggle
                return client.send(req, res);
            }
            else
            {
                httplib::Client client(this->url->host().c_str(), this->url->port());
                return client.send(req, res);
            }
        }

    private:
        const Url::Url *url;
    };

    // create http client instance with parsed URL
    ClientWrapper c(this->_url.get());

    // compose http request
    httplib::Request req;

    // make headers for httplib
    for (auto&& h : this->_headers)
    {
        req.set_header(h.first.c_str(), h.second.c_str());
    }

    // set http request method
    req.method = this->_method;

    // set http request path
    req.path = this->_url->path();

    // set request body
    req.body = this->_data;

    auto res = std::make_shared<httplib::Response>();

    if (c.send(req, *res))
    {
        // create response object
        Response response;
        response.setSuccess(true);
        response.setStatus(res->status);

        std::multimap<std::string, std::string> headers;
        for (auto&& h : res->headers)
        {
            headers.insert(h);
        }
        response.setHeaders(headers);

        response.setBody(res->body);

        return response;
    }

    // create invalid empty response
    LOG_ERROR("Request: server request failed!");
    return Response();
}
