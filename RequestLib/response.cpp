#include <requestlib/response.hpp>

#include <algorithm>

Response::Response()
{
}

Response::~Response()
{
}

bool Response::hasHeader(const std::string &header) const
{
    std::string copy = header;
    std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
    return this->_headers.count(copy) > 0;
}

const std::list<std::string> Response::getHeaderValues(const std::string &header) const
{
    if (this->hasHeader(header))
    {
        std::string copy = header;
        std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);

        std::list<std::string> values;
        for (auto&& h : this->_headers)
        {
            if (h.first == copy)
            {
                values.emplace_back(h.second);
            }
        }
        return values;
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

void Response::setHeaders(const std::multimap<std::string, std::string> &headers)
{
    this->_headers = headers;
}

void Response::setBody(const std::string &data)
{
    this->_data = data;
}
