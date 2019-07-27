#include <requestlib/response.hpp>

#include <algorithm>

#include <utils/string.hpp>
#include <utils/list.hpp>

Response::Response()
{
}

Response::~Response()
{
}

bool Response::hasHeader(const std::string &header) const
{
    return list::strcontains_key(this->_headers, header, string::case_insensitive_compare);
}

const std::list<std::string> Response::getHeaderValues(const std::string &header) const
{
    if (this->hasHeader(header))
    {
        return list::mm_values(this->_headers, header, string::case_insensitive_compare);
    }

    return {};
}

const std::string Response::getHeaderValue(const std::string &header) const
{
    const auto res = this->getHeaderValues(header);
    if (!res.empty())
    {
        return res.front();
    }
    return {};
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

void Response::setHeaders(const HeaderMap &headers)
{
    this->_headers = headers;
}

void Response::setBody(const std::string &data)
{
    this->_data = data;
}
