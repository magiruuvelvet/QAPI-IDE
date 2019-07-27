#ifndef REQUESTLIB_REQUEST_HPP
#define REQUESTLIB_REQUEST_HPP

#include <string>
#include <memory>

#include <utils/list.hpp>

#include <requestlib/response.hpp>
#include <requestlib/methods.hpp>

#include <requestlib/exceptions.hpp>

// forward declare url library (private)
namespace Url {
    struct Url;
}

class Request final
{
public:
    Request(const std::string &url, RequestMethod method = RequestMethod::GET, const std::string &custom_method = {});
    Request(const std::string &url, const std::string &method);
    ~Request();

    using HeaderMap = list::ci_multimap<std::string, std::string>;

    // returns the original input URL
    inline const auto &urlString() const
    { return this->_full_url; }

    // returns the parsed URL
    const std::string url() const;

    // returns the request method as string
    inline const auto &method() const
    { return this->_method; }

    // returns a string map of all currently set headers
    inline const auto &headers() const
    { return this->_headers; }

    // returns the current request body
    inline const auto &body() const
    { return this->_data; }

    // whenever to verify the TLS certificate or not
    // defaults to false for local HTTPS development (promotion: mkcert on GitHub)
    void verifyCertificate(bool enabled);

    // whenever to follow HTTP redirects (Location header)
    // defaults to false
    void followRedirects(bool enabled);

    // sets the maximum redirect attempts before throwing a TooManyRedirects exception
    // defaults to 10 attempts
    void setMaxRedirectAttempts(std::uint16_t attempts);

    // set UTF-8 encoded string data
    void setRequestBody(const std::string &data);

    // appends UTF-8 encoded string data to the body instead of overwriting it
    void appendToRequestBody(const std::string &data);

    // adds a HTTP header to the request
    void setHeader(const std::string &header, const std::string &value);

    // check if a specific HTTP header is present
    bool hasHeader(const std::string &header) const;

    // receive the values of all HTTP headers with the given name
    const std::list<std::string> getHeaderValues(const std::string &header) const;

    // receive the first occurrence of the given HTTP header
    // calls getHeaderValues() and returns the first result if any
    const std::string getHeaderValue(const std::string &header) const;

    // remove a header from the request
    void removeHeader(const std::string &header);

    // perform the HTTP request and receive a response object
    const Response performRequest();

private:
    // disable assignment
    void operator=(const Request&) = delete;
    void operator=(Request&) = delete;

    Request(const std::string &url, const std::string &method,
            const HeaderMap &headers, const std::string &data,
            std::uint16_t current_redirect_count, std::uint16_t max_redirect_attempts);

    std::string _full_url;
    std::string _method;
    bool _verifyCertificate = false;
    bool _followRedirects = false;

    std::shared_ptr<Url::Url> _url;
    HeaderMap _headers;
    std::string _data;

    std::uint16_t _redirect_count = 0;
    std::uint16_t _max_redirect_attempts = 10;
};

#endif // REQUESTLIB_REQUEST_HPP
