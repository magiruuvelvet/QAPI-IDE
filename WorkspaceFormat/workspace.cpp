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

void Workspace::Request::removeHeader(const std::string &header)
{
    if (this->hasHeader(header))
    {
        this->_headers.erase(header);
    }
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

bool Workspace::Request::hasHeader(const std::string &headerName) const
{
    return list::strcontains_key(this->_headers, headerName, string::case_insensitive_compare);
}

const std::list<std::string> Workspace::Request::headerValues(const std::string &headerName) const
{
    if (this->hasHeader(headerName))
    {
        return list::mm_values(this->_headers, headerName, string::case_insensitive_compare);
    }

    return {};
}

const std::list<std::string> Workspace::getRequestGroupNames() const
{
    std::list<std::string> names;
    for(auto&& group : this->_data.requestGroups)
    {
        names.emplace_back(group.name());
    }
    return names;
}

const std::string Workspace::serializeJson() const
{
    json output;
    this->serializeInternal(&output);
    return output.dump();
}

const std::vector<uint8_t> Workspace::serialize() const
{
    json output;
    this->serializeInternal(&output);
    return json::to_cbor(output);
}

void Workspace::serializeInternal(void *outputJsonObj) const
{
    json &output = *static_cast<json*>(outputJsonObj);

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
            json headers = json::object();
            for (auto&& header : req.headers())
            {
                headers[header.first].push_back(header.second);
            }

            requests.push_back({
                {"name", req.name()},
                {"url", req.url()},
                {"headers", headers},
            });
        }

        group_json["requests"] = requests;
        requestgroups.push_back(group_json);
    }

    workspace["requestgroups"] = requestgroups;

    output = workspace;
}

Workspace Workspace::deserialize(const std::string &json, bool *success)
{
    // deserialize workspace with the json deserializer
    Workspace workspace;
    deserializeInternal(&workspace, json, success);
    return workspace;
}

Workspace Workspace::deserialize(const std::vector<std::uint8_t> &cbor, bool *success)
{
    // deserialize CBOR format and store as json
    const auto json = json::from_cbor(cbor, true, false);

    // check for deserialization errors without using an exception
    if (json.is_discarded())
    {
        success ? *success = false : bool();
        return {};
    }

    // continue deserialization with the internal json deserializer
    Workspace workspace;
    deserializeInternal(&workspace, json, success);
    return workspace;
}

void Workspace::deserializeInternal(Workspace *output, const std::string &json_str, bool *success)
{
    // deserialize JSON string and store as json object
    const auto json = json::parse(json_str, nullptr, false);

    // check for deserialization errors without using an exception
    if (json.is_discarded())
    {
        success ? *success = false : bool();
        return;
    }

    // json library is exception based, continue parsing
    // until we hit an error and keep an incomplete workspace object
    try
    {
        // get workspace name
        output->_data.name = json.at("name").get<std::string>();

        // get request groups
        const auto requestgroups = json.at("requestgroups");
        if (!requestgroups.is_array())
        {
            throw;
        }

        for (auto&& group : requestgroups)
        {
            RequestGroup gr(group.at("name").get<std::string>());

            // get requests
            const auto requests = group.at("requests");
            if (!requests.is_array())
            {
                throw;
            }

            for(auto&& request : requests)
            {
                Request req(
                    request.at("name").get<std::string>(),
                    request.at("url").get<std::string>()
                );

                // get request headers
                const auto headers = request.at("headers");
                if (!headers.is_object())
                {
                    throw;
                }

                for (auto&& header : headers.items())
                {
                    if (!header.value().is_array())
                    {
                        throw;
                    }

                    for (auto&& value : header.value())
                    {
                        req.addHeader(header.key(), value.get<std::string>());
                    }
                }

                gr.addRequest(req);
            }

            output->addRequestGroup(gr);
        }

        // set success status
        success ? *success = true : bool();
    }
    catch (...)
    {
        // set failure status
        success ? *success = false : bool();
        return;
    }
}
