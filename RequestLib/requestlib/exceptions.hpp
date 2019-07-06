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
    {}

    virtual ~UrlParseException() override
    {}

    const char *what() const noexcept override
    { return msg.c_str(); }

private:
    const std::string msg;
};

class TooManyRedirects : public std::runtime_error
{
public:
    explicit TooManyRedirects(const std::string &msg)
        : std::runtime_error(msg),
          msg(msg)
    {}

    virtual ~TooManyRedirects() override
    {}

    const char *what() const noexcept override
    { return msg.c_str(); }

private:
    const std::string msg;
};

class InvalidRedirect : public std::runtime_error
{
public:
    explicit InvalidRedirect(const std::string &msg)
        : std::runtime_error(msg),
          msg(msg)
    {}

    virtual ~InvalidRedirect() override
    {}

    const char *what() const noexcept override
    { return msg.c_str(); }

private:
    const std::string msg;
};

#endif // REQUESTLIB_EXCEPTIONS_HPP
