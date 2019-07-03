#ifndef REQUESTLIB_CURL_HPP
#define REQUESTLIB_CURL_HPP

#include <memory>

struct libcurl final
{
public:
    libcurl();
    ~libcurl();

    static inline auto *self()
    { return _self.get(); }

    // check if libcurl is initialized or not
    static inline auto isInitialized()
    { return libcurl::_is_initialized; }

    // get a new curl handle
    static void *getNewHandle(const std::string &url);

    // clean up curl handle
    static void cleanupHandle(void *curl_handle);

    // perform a request using the given handle
    static int performRequest(void *curl_handle);

private:
    static std::unique_ptr<libcurl> _self;
    static bool _is_initialized;
};

#endif // REQUESTLIB_CURL_HPP
