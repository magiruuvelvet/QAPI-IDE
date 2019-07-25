#include <workspace/workspace.hpp>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Workspace::Workspace()
{
}

Workspace::Workspace(const std::string &name)
{
    this->_data.name = name;
}

Workspace::~Workspace()
{
}

void Workspace::addRequestGroup(const RequestGroup &group)
{
    this->_data.requestGroups.emplace_back(group);
    this->_data.requestGroups.back()._parent = this;
}

void Workspace::RequestGroup::addRequest(const Request &request)
{
    this->_requests.emplace_back(request);
    this->_requests.back()._parent = this;
}

void Workspace::Request::addHeader(const std::string &header, const std::string &value)
{
    this->_headers.insert(std::pair{header, value});
}

void Workspace::Request::removeHeader(const std::string &header, const std::string &value)
{
    auto iterator_pair = this->_headers.equal_range(header);
    auto it = iterator_pair.first;
    for (; it != iterator_pair.second; ++it)
    {
        if (it->second == value)
        {
            this->_headers.erase(it);
            break;
        }
    }
}

const std::vector<uint8_t> Workspace::serialize() const
{
    json workspace = {
        {"name", this->_data.name},
    };

    json requestgroups = json::array();
    for (auto&& group : this->_data.requestGroups)
    {
        json requests = json::array();

        json group_json = {
            {"name", group.name()},
        };

        for (auto&& req : group.requests())
        {
            requests.push_back({
                {"name", req.name()},
                {"url", req.url()},
                // TODO: headers
            });
        }

        group_json["requests"] = requests;
        requestgroups.push_back(group_json);
    }

    workspace["requestgroups"] = requestgroups;

    return json::to_cbor(workspace);
}
