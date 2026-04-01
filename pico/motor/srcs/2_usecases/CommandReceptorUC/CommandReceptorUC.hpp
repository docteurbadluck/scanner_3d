#pragma once

#include "1_domain/System.hpp"
#include "3_interface/IReceptor.hpp"
#include <string>

class CommandReceptorUC
{
private:
    IReceptor &_receptor;
    std::string _message;
    Commands    _cmds;

public:
    CommandReceptorUC(IReceptor &recep, Commands cmds);
    ~CommandReceptorUC() = default;

    void        check_for_new_message(System &sys);
    bool        interprete_message(System &sys);
    std::string get_message();
};