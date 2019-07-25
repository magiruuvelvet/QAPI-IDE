#ifndef WORKSPACEFORMAT_WORKSPACE_HPP
#define WORKSPACEFORMAT_WORKSPACE_HPP

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <list>

class Workspace
{
public:
    Workspace();
    Workspace(const std::string &name);
    ~Workspace();

    inline void setName(const std::string &name)
    { this->_data.name = name; }
    inline const auto &name() const
    { return this->_data.name; }

    struct RequestGroup;
    struct WorkspaceData final
    {
        friend Workspace;

    private:
        std::string name;
        std::list<RequestGroup> requestGroups;
    };

    struct Request;
    struct RequestGroup final
    {
        friend Workspace;

    public:
        RequestGroup() {}
        RequestGroup(const std::string &name)
            : _name(name)
        {}

        inline void setName(const std::string &name)
        { this->_name = name; }
        inline const auto &name() const
        { return this->_name; }

        void addRequest(const Request &request);
        inline const auto &requests() const
        { return this->_requests; }

        inline const Workspace *parent() const
        { return this->_parent; }

    private:
        const Workspace *_parent = nullptr;

        std::string _name;
        std::list<Request> _requests;
    };

    struct Request final
    {
        friend RequestGroup;

    public:
        Request() {}
        Request(const std::string &name)
            : _name(name)
        {}
        Request(const std::string &name, const std::string &url)
            : _name(name),
              _url(url)
        {}

        inline void setName(const std::string &name)
        { this->_name = name; }
        inline const auto &name() const
        { return this->_name; }

        inline void setUrl(const std::string &url)
        { this->_url = url; }
        inline const auto &url() const
        { return this->_url; }

        void addHeader(const std::string &header, const std::string &value);
        void removeHeader(const std::string &header, const std::string &value);
        inline const auto &headers() const
        { return this->_headers; }

        inline const RequestGroup *parent() const
        { return this->_parent; }

    private:
        const RequestGroup *_parent = nullptr;

        std::string _name;
        std::string _url;
        std::multimap<std::string, std::string> _headers;
    };

    void addRequestGroup(const RequestGroup &group);
    inline const auto &requestGroups() const
    { return this->_data.requestGroups; }

    const std::vector<std::uint8_t> serialize() const;

private:
    WorkspaceData _data;
};

#endif // WORKSPACEFORMAT_WORKSPACE_HPP
