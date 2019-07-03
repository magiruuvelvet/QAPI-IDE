#include <requestlib/request.hpp>
#include <requestlib/curl.hpp>

#include <logger/log.hpp>

#include <cstring>

extern "C" {
    #include <curl/curl.h>
}

Request::Request(const std::string &url, RequestMethod method, const std::string &custom_method)
    : _url(url)
{
    // assign the request method as string
    if (method != RequestMethod::CUSTOM)
    {
        // cast enum name to string
        //this->_method = ~method;
    }
    else
    {
        this->_method = custom_method;
    }
}

Request::~Request()
{
}

void Request::setRequestBody(const std::string &data)
{
    this->_data = std::make_shared<char>(*strdup(data.c_str()));
}

void Request::setRequestBody(const char *data)
{
    this->_data = std::make_shared<char>(*strdup(data));
}

void Request::setHeader(const std::string &header, const std::string &value)
{
    LOG_TODO("Request: setHeader(const std::string &header, const std::string &value)");
}

bool Request::performRequest()
{
    // TODO: request method, request body, response data handling, ...
    LOG_TODO("Request: performRequest() is incomplete!");

    auto handle = libcurl::getNewHandle(this->_url);

    // FIXME: response is straight spammed into terminal, need this as string though...
    auto ret = libcurl::performRequest(handle);

    if (ret == CURLE_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}
