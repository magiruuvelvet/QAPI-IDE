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

    inline const auto &method() const
    { return this->_method; }

    inline const auto &headers() const
    { return this->_headers; }

    // set UTF-8 encoded string data
    void setRequestBody(const std::string &data);

    // adds a HTTP header to the request
    void setHeader(const std::string &header, const std::string &value);

    // perform the HTTP request and receive a response object
    Response performRequest();

private:
    std::string _full_url;
    std::string _method;

    std::shared_ptr<Url::Url> _url;
    std::map<std::string, std::string> _headers;
    std::string _data;
};

#endif // REQUESTLIB_REQUEST_HPP
