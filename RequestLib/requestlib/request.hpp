#ifndef REQUESTLIB_REQUEST_HPP
#define REQUESTLIB_REQUEST_HPP

#include <string>
#include <memory>
#include <map>

#include <requestlib/response.hpp>
#include <requestlib/methods.hpp>

// forward declare url library
namespace Url {
    struct Url;
}

class Request final
{
public:
    Request(const std::string &url, RequestMethod method = RequestMethod::GET, const std::string &custom_method = {});
    ~Request();

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

    // set UTF-8 encoded string data
    void setRequestBody(const std::string &data);

    // appends UTF-8 encoded string data to the body instead of overwriting it
    void appendToRequestBody(const std::string &data);

    // adds a HTTP header to the request (overwrites existing headers)
    void setHeader(const std::string &header, const std::string &value);

    // check if a specific HTTP header is present
    bool hasHeader(const std::string &header) const;

    // receive the value of a HTTP header
    const std::string getHeaderValue(const std::string &header) const;

    // remove a header from the request
    void removeHeader(const std::string &header);

    // perform the HTTP request and receive a response object
    const Response performRequest();

private:
    std::string _full_url;
    std::string _method;
    bool _verifyCertificate = false;

    std::shared_ptr<Url::Url> _url;
    std::map<std::string, std::string> _headers;
    std::string _data;
};

#endif // REQUESTLIB_REQUEST_HPP
