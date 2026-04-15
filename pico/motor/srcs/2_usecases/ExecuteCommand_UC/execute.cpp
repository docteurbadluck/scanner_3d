#include "ExecuteCommand_UC.hpp"

void ExecuteCommand_UC::execute(System &sys)
{
    std::string cmd = sys.getCommandToExecute();
    int res = _dispatch(cmd);
    sys.resultCommand(res);
    _sender.sendResponse(cmd, res == 0, sys);
}
