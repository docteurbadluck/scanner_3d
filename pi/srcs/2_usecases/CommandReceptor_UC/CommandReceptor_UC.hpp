#pragma once
#include "1_domain/System.hpp"
#include "2_usecases/SendToComputer_UC/SendToComputer_UC.hpp"
#include "3_interface/IComputer.hpp"

class CommandReceptor_UC
{
private:
    IComputer     &_computer;
    SendToComputer_UC &_sender;
    std::string        _message;

    void _setMessageFrom(const std::string &raw);
    bool _isValidCommand(const Commands &cmds) const;
public:
    CommandReceptor_UC(IComputer &uploader, SendToComputer_UC &sender);
    ~CommandReceptor_UC() = default;

    bool        waitAndInterprete(System &sys);
    std::string getMessage() const;
};
