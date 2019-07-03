#include <requestlib/response.hpp>

Response::Response()
{
}

Response::~Response()
{
}

void Response::setSuccess(bool success)
{
    this->_success = success;
}

void Response::setStatus(int status)
{
    this->_status = status;
}

void Response::setVersion(const std::string &version)
{
    this->_version = version;
}

void Response::setHeaders(const std::multimap<std::string, std::string> &headers)
{
    this->_headers = headers;
}

void Response::setBody(const std::string &data)
{
    this->_data = data;
}
