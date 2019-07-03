#ifndef REQUESTLIB_RESPONSE_HPP
#define REQUESTLIB_RESPONSE_HPP

#include <string>
#include <map>

class Response final
{
    friend class Request;

public:
    ~Response();

    // returns true when the network request was successful
    // or false when a network or transport error occurred
    inline auto successful() const
    { return this->_success; }

    // returns the HTTP status code
    inline auto status() const
    { return this->_status; }

    // returns the HTTP version
    inline const auto &version() const
    { return this->_version; }

    // returns a map of all headers
    inline const auto &headers() const
    { return this->_headers; }

    // returns the response body
    inline const auto &body() const
    { return this->_data; }

private:
    Response();

    void setSuccess(bool);
    void setStatus(int);
    void setVersion(const std::string &version);
    void setHeaders(const std::multimap<std::string, std::string> &headers);
    void setBody(const std::string &data);

    // if the network request was successful or not
    // doesn't indicate other errors
    bool _success = false;

    int _status = 0;
    std::string _version;
    std::string _data;
    std::multimap<std::string, std::string> _headers;
};

#endif // REQUESTLIB_RESPONSE_HPP
