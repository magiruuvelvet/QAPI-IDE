#ifndef SCRIPTING_EXCEPTIONS_HPP
#define SCRIPTING_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

class UnsupportedLanguageBackend : public std::runtime_error
{
public:
    explicit UnsupportedLanguageBackend(const std::string &msg)
        : std::runtime_error(msg),
          msg(msg)
    {}

    virtual ~UnsupportedLanguageBackend() override
    {}

    const char *what() const noexcept override
    { return msg.c_str(); }

private:
    const std::string msg;
};

class UnsupportedDataType : public std::runtime_error
{
public:
    explicit UnsupportedDataType(const std::string &msg)
        : std::runtime_error(msg),
          msg(msg)
    {}

    virtual ~UnsupportedDataType() override
    {}

    const char *what() const noexcept override
    { return msg.c_str(); }

private:
    const std::string msg;
};

#endif // SCRIPTING_EXCEPTIONS_HPP
