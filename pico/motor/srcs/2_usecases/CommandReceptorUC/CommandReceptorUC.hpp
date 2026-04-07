#pragma once
#include "1_domain/System/System.hpp"
#include "3_interface/IReceptor.hpp"
#include "2_usecases/SendToPi_UC/SendToPi_UC.hpp"
#include <string>

class CommandReceptorUC
{
private:
    IReceptor   &_receptor;
    Commands     _cmds;
    SendToPi_UC &_sender;
    std::string  _message;

    void _handleValidCommand(System &sys);

public:
    CommandReceptorUC(IReceptor &recep, Commands cmds, SendToPi_UC &sender);
    ~CommandReceptorUC() = default;

    void        checkForNewMessage(System &sys);
    bool        interpreteMessage(System &sys);
    std::string getMessage();
};
