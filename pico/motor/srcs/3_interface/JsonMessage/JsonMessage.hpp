#pragma once
#include <string>

namespace JsonMessage
{
std::string escape(const std::string &value);
std::string makeCommand(const std::string &command);
std::string makeState(const std::string &state);
std::string makeResponse(const std::string &kind, const std::string &command);
std::string makeError(const std::string &reason);
std::string makeTestHardwareResponse(const std::string &dc, const std::string &servo, const std::string &stepper);
std::string extractStringField(const std::string &json, const std::string &field);
}
