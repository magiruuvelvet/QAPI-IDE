#ifndef REQUESTLIB_REQUEST_HPP
#define REQUESTLIB_REQUEST_HPP

#include <string>
#include <memory>

#include <requestlib/methods.hpp>

class Request
{
public:
    Request(const std::string &url, RequestMethod method = RequestMethod::GET, const std::string &custom_method = {});
    ~Request();

    inline const auto &url() const
    { return this->_url; }

    inline const auto &method() const
    { return this->_method; }

    // set UTF-8 encoded string data
    void setRequestBody(const std::string &data);

    // set any NULL-terminated data
    void setRequestBody(const char *data);

    // adds a HTTP header to the request
    void setHeader(const std::string &header, const std::string &value);

    // TODO: error enum
    bool performRequest();

private:
    std::string _url;
    std::string _method;

    // standalone copy of the data
    std::shared_ptr<char> _data;
};

#endif // REQUESTLIB_REQUEST_HPP
