#pragma once
#include <string>
#include "1_domain/System.hpp"
#include "3_interface/ISender.hpp"

class SendToPi_UC
{
private:
    ISender &_sender;

public:
    SendToPi_UC(ISender &sender);
    ~SendToPi_UC() = default;

    void sendResponse(const std::string &cmd, bool success, const System &sys);
    void sendState(const System &sys);
    void sendInvalidCmd();
};
