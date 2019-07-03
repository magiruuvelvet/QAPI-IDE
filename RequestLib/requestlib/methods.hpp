#ifndef REQUESTLIB_METHODS_HPP
#define REQUESTLIB_METHODS_HPP

#include <string>

/**
 * List of common HTTP request methods.
 */

enum class RequestMethod {
    GET,
    POST,
    PUT,
    PATCH,
    DELETE,
    COPY,
    HEAD,
    OPTIONS,
    LINK,
    UNLINK,
    PURGE,
    LOCK,
    UNLOCK,
    PROPFIND,
    VIEW,

    CUSTOM, // used for custom request methods
};

const std::string RequestMethodString(RequestMethod, const std::string &custom = {});

#endif // REQUESTLIB_METHODS_HPP
