#ifndef WORKSPACEFORMAT_WORKSPACE_HPP
#define WORKSPACEFORMAT_WORKSPACE_HPP

#include <string>
#include <map>
#include <vector>
#include <list>

#include <utils/list.hpp>

class Workspace
{
public:
    Workspace();
    Workspace(const std::string &name);
    ~Workspace() = default;

    using HeaderMap = list::ci_multimap<std::string, std::string>;

    // workspace empty check
    inline bool isEmtpy() const
    {
        return this->_data.name.empty() &&
               this->_data.requestGroups.empty();
    }

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
        Request(const std::string &name, const std::string &url,
                const HeaderMap &headers)
            : _name(name),
              _url(url),
              _headers(headers)
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
        void removeHeader(const std::string &header);
        void removeHeader(const std::string &header, const std::string &value);
        inline const auto &headers() const
        { return this->_headers; }

        bool hasHeader(const std::string &headerName) const;
        const std::list<std::string> headerValues(const std::string &headerName) const;

        inline const RequestGroup *parent() const
        { return this->_parent; }

    private:
        const RequestGroup *_parent = nullptr;

        std::string _name;
        std::string _url;
        HeaderMap _headers;
    };

    void addRequestGroup(const RequestGroup &group);
    inline const auto &requestGroups() const
    { return this->_data.requestGroups; }

    const std::string serializeJson() const;
    const std::vector<std::uint8_t> serialize() const;

    static Workspace deserialize(const std::string &json, bool *success = nullptr);
    static Workspace deserialize(const std::vector<std::uint8_t> &cbor, bool *success = nullptr);

private:
    WorkspaceData _data;

    void serializeInternal(void *outputJsonObj) const;
    static void deserializeInternal(Workspace *output, const std::string &json, bool *success = nullptr);
};

#endif // WORKSPACEFORMAT_WORKSPACE_HPP
