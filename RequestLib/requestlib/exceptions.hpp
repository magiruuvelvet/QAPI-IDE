#ifndef REQUESTLIB_EXCEPTIONS_HPP
#define REQUESTLIB_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

class UrlParseException : public std::logic_error
{
public:
    explicit UrlParseException(const std::string &msg)
        : std::logic_error(msg),
          msg(msg)
    {
    }

    virtual ~UrlParseException() override
    {}

    const char *what() const noexcept override
    {
        return msg.c_str();
    }

private:
    const std::string msg;
};

#endif // REQUESTLIB_EXCEPTIONS_HPP
