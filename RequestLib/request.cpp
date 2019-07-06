#include <requestlib/request.hpp>

#include <algorithm>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_ZLIB_SUPPORT
#include <httplib.h>

#include <url.h>

Request::Request(const std::string &url, RequestMethod method, const std::string &custom_method)
    : _full_url(url)
{
    // set request method as string
    this->_method = RequestMethodString(method, custom_method);

    // don't do anything on empty url
    if (url.empty())
    {
        return;
    }

    // parse url
    try {
        this->_url = std::make_shared<Url::Url>(url);
    } catch (Url::UrlParseException &e) {
        this->_url = nullptr;
        // throw custom exception with same name and error message
        // the url lib is not visible in the other modules of the application
        // and the public interface to avoid cluttering the global scope
        throw UrlParseException(e.what());
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

//#ifdef DEBUG_BUILD
//    LOG_INFO("{}, {}", *this, *this->_url);
//    LOG_INFO(
//              "Scheme:     {}\n"
//        "      Userinfo:   {}\n"
//        "      Host:       {}\n"
//        "      Port:       {}\n"
//        "      Path:       {}\n"
//        "      Params:     {}\n"
//        "      Query:      {}\n"
//        "      Fragment:   {}\n",
//        this->_url->scheme(),
//        this->_url->userinfo(),
//        this->_url->host(),
//        this->_url->port(),
//        this->_url->path(),
//        this->_url->params(),
//        this->_url->query(),
//        this->_url->fragment(),
//        this->_url->str()
//    );
//#endif
}

Request::Request(const std::string &url, const std::string &method)
    : Request(url, RequestMethod::CUSTOM, method)
{
}

Request::Request(const std::string &url, const std::string &method,
                 const std::map<std::string, std::string> &headers, const std::string &data,
                 uint16_t current_redirect_count, uint16_t max_redirect_attempts)
    : Request(url, method)
{
    // this is a private constructor to recursively follow redirects
    this->_followRedirects = true;
    this->_redirect_count = current_redirect_count;
    this->_max_redirect_attempts = max_redirect_attempts;

    // transfer headers and body
    this->_headers = headers;
    this->_data = data;
}

Request::~Request()
{
}

const std::string Request::url() const
{
    return this->_url ? this->_url->str() : "";
}

void Request::verifyCertificate(bool enabled)
{
    this->_verifyCertificate = enabled;
}

void Request::followRedirects(bool enabled)
{
    this->_followRedirects = enabled;
}

void Request::setMaxRedirectAttempts(std::uint16_t attempts)
{
    this->_max_redirect_attempts = attempts;
}

void Request::setRequestBody(const std::string &data)
{
    this->_data = data;
}

void Request::appendToRequestBody(const std::string &data)
{
    this->_data.append(data);
}

void Request::setHeader(const std::string &header, const std::string &value)
{
    std::string copy = header;
    std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
    this->_headers.insert(std::pair<std::string, std::string>(copy, value));
}

bool Request::hasHeader(const std::string &header) const
{
    std::string copy = header;
    std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
    return this->_headers.count(copy) > 0;
}

const std::string Request::getHeaderValue(const std::string &header) const
{
    if (this->hasHeader(header))
    {
        std::string copy = header;
        std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
        return this->_headers.at(copy);
    }

    return {};
}

void Request::removeHeader(const std::string &header)
{
    if (this->hasHeader(header))
    {
        std::string copy = header;
        std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
        this->_headers.erase(this->_headers.find(copy));
    }
}

const Response Request::performRequest()
{
    // return invalid response on empty url
    if (this->_full_url.empty())
    {
        return Response();
    }

    struct ClientWrapper {
        ClientWrapper(const Url::Url *url, bool verifyCertificate)
            : url(url),
              verifyCertificate(verifyCertificate)
        {
        }

        // wrapper around Client and SSLClient to support both at the same time
        bool send(httplib::Request &req, httplib::Response &res)
        {
            if (this->url->scheme() == "https")
            {
                httplib::SSLClient client(this->url->host().c_str(), this->url->port());
                client.enable_server_certificate_verification(this->verifyCertificate);
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
        bool verifyCertificate = false;
    };

    // create http client instance with parsed URL
    ClientWrapper c(this->_url.get(), this->_verifyCertificate);

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

    const auto res = std::make_shared<httplib::Response>();

    if (c.send(req, *res))
    {
        // check if to follow redirects
        if (this->_followRedirects)
        {
            if (res->status >= 301 && res->status <= 303)
            {
                std::string new_url;

                if (res->has_header("Location"))
                {
                    const auto location = res->get_header_value("Location");

                    // location header is empty or doesn't exist
                    if (location.empty())
                    {
                        throw InvalidRedirect("got redirect status, but no location header");
                    }

                    if (location.rfind("http://", 0) == 0 || location.rfind("https://", 0) == 0)
                    {
                        // new url becomes absolute
                        new_url = location;
                    }
                    else
                    {
                        // set path of current url
                        auto tmp_url = *this->_url.get();
                        tmp_url.setPath(location);
                        new_url = tmp_url.str();
                    }
                }

                // check redirect count
                if (this->_redirect_count >= this->_max_redirect_attempts)
                {
                    throw TooManyRedirects("reached maximum redirect attempts");
                }

                // increase redirect attempts
                this->_redirect_count++;

                // recursively perform requests until the result is no longer 3xx (redirect)
                // or too many attempts where performed
                return Request(new_url, this->_method,
                               this->_headers, this->_data,
                               this->_redirect_count, this->_max_redirect_attempts)
                        .performRequest();
            }
        }

        // create response object
        Response response;
        response.setSuccess(true);
        response.setStatus(res->status);

        std::multimap<std::string, std::string> headers;
        for (auto&& h : res->headers)
        {
            std::string copy = h.first;
            std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
            headers.insert(std::pair<std::string, std::string>(copy, h.second));
        }
        response.setHeaders(headers);

        response.setBody(res->body);

        return response;
    }

    // create invalid empty response
    return Response();
}
