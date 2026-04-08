#pragma once
#include <string>
#include "1_domain/System/System.hpp"
#include "3_interface/ISender.hpp"

class SendToComputer_UC
{
private:
    ISender &_sender;

public:
    SendToComputer_UC(ISender &sender);
    ~SendToComputer_UC() = default;

    void sendResponse(const std::string &cmd, bool success, const System &sys);
    void sendState(const System &sys);
    void sendInvalidCmd();
    void sendRaw(const std::string &message);
};
