#ifndef REQUESTLIB_METHODS_HPP
#define REQUESTLIB_METHODS_HPP

//#include <enum.hpp>

/**
 * List of common HTTP request methods.
 */

//DECLARE_ENUM_WITH_TYPE(RequestMethod, int,
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
//);
};

#endif // REQUESTLIB_METHODS_HPP
