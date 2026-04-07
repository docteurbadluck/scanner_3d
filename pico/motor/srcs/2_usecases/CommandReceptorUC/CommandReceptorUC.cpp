#include "CommandReceptorUC.hpp"

CommandReceptorUC::CommandReceptorUC(IReceptor &recep, Commands cmds, SendToPi_UC &sender)
    : _receptor(recep), _cmds(cmds), _sender(sender)
{}
