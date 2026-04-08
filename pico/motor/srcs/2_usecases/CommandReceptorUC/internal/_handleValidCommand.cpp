#include "../CommandReceptorUC.hpp"

void CommandReceptorUC::_handleValidCommand(System &sys)
{
    sys.commandInterpreted(true);
    sys.setCommandToExecute(_message);
    _sender.sendState(sys);
}
