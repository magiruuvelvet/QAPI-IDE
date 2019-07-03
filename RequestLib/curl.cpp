#include <requestlib/curl.hpp>

#include <logger/log.hpp>

extern "C" {
    #include <curl/curl.h>
}

// initialize instance
auto libcurl::_self = std::make_unique<libcurl>();
auto libcurl::_is_initialized = false;

libcurl::libcurl()
{
    if (libcurl::_is_initialized)
    {
        return;
    }

    auto ret = curl_global_init(CURL_GLOBAL_ALL);
    if (ret != CURLE_OK)
    {
        LOG_ERROR("libcurl: unable to initialize curl! status = %i", ret);
    }
    else
    {
        libcurl::_is_initialized = true;
    }
}

libcurl::~libcurl()
{
    if (!libcurl::_is_initialized)
    {
        return;
    }

    curl_global_cleanup();
    libcurl::_is_initialized = false;
}

void *libcurl::getNewHandle(const std::string &url)
{
    if (libcurl::_is_initialized)
    {
        auto handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        return handle;
    }
    else
    {
        LOG_ERROR("libcurl: unable to obtain a new handle! curl is not initialized!");
        return nullptr;
    }
}

void libcurl::cleanupHandle(void *curl_handle)
{
    if (libcurl::_is_initialized)
    {
        curl_easy_cleanup(curl_handle);
    }
    else
    {
        LOG_ERROR("libcurl: unable to clean up handle! curl is not initialized!");
    }
}

int libcurl::performRequest(void *curl_handle)
{
    // FIXME: put response data into string
    return static_cast<int>(curl_easy_perform(curl_handle));
}
