#include "1_domain/JsonMessage/JsonMessage.hpp"

namespace
{
static size_t find_value_start(const std::string &json, const std::string &key)
{
    const size_t keyPos = json.find(key);
    if (keyPos == std::string::npos)
        return std::string::npos;
    const size_t colonPos = json.find(':', keyPos + key.size());
    if (colonPos == std::string::npos)
        return std::string::npos;
    return json.find('"', colonPos + 1);
}

static std::string extract_value_from(const std::string &json, size_t start)
{
    std::string value;
    bool        escaped = false;
    for (size_t i = start + 1; i < json.size(); ++i)
    {
        const char c = json[i];
        if (escaped) { value += c; escaped = false; continue; }
        if (c == '\\') { escaped = true; continue; }
        if (c == '"') return value;
        value += c;
    }
    return "";
}
}

namespace JsonMessage
{
std::string escape(const std::string &value)
{
    std::string escaped;
    escaped.reserve(value.size());
    for (char c : value)
    {
        if (c == '\\' || c == '"')
            escaped += '\\';
        escaped += c;
    }
    return escaped;
}

std::string makeCommand(const std::string &command)
{
    return std::string{"{\"type\":\"command\",\"command\":\""}
           + escape(command) + "\"}";
}

std::string makeState(const std::string &state)
{
    return std::string{"{\"type\":\"state\",\"state\":\""}
           + escape(state) + "\"}";
}

std::string makeResponse(const std::string &kind, const std::string &command)
{
    return std::string{"{\"type\":\"response\",\"kind\":\""}
           + escape(kind) + "\",\"command\":\"" + escape(command) + "\"}";
}

std::string makeError(const std::string &reason)
{
    return std::string{"{\"type\":\"error\",\"reason\":\""}
           + escape(reason) + "\"}";
}

std::string extractStringField(const std::string &json, const std::string &field)
{
    const std::string key = std::string{"\""} + field + "\"";
    const size_t start = find_value_start(json, key);
    if (start == std::string::npos)
        return "";
    return extract_value_from(json, start);
}
}