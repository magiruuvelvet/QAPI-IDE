#include <requestlib/methods.hpp>

const std::string RequestMethodString(RequestMethod method, const std::string &custom)
{
    switch (method)
    {
        case RequestMethod::GET:       return "GET";      break;
        case RequestMethod::POST:      return "POST";     break;
        case RequestMethod::PUT:       return "PUT";      break;
        case RequestMethod::PATCH:     return "PATCH";    break;
        case RequestMethod::DELETE:    return "DELETE";   break;
        case RequestMethod::COPY:      return "COPY";     break;
        case RequestMethod::HEAD:      return "HEAD";     break;
        case RequestMethod::OPTIONS:   return "OPTIONS";  break;
        case RequestMethod::LINK:      return "LINK";     break;
        case RequestMethod::UNLINK:    return "UNLINK";   break;
        case RequestMethod::PURGE:     return "PURGE";    break;
        case RequestMethod::LOCK:      return "LOCK";     break;
        case RequestMethod::UNLOCK:    return "UNLOCK";   break;
        case RequestMethod::PROPFIND:  return "PROPFIND"; break;
        case RequestMethod::VIEW:      return "VIEW";     break;
        case RequestMethod::CUSTOM:    return custom;     break;
    }
}
