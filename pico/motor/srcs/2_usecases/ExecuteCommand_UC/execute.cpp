#include "ExecuteCommand_UC.hpp"

void ExecuteCommand_UC::execute(System &sys)
{
    std::string cmd = sys.getCommandToExecute();
    int res = _dispatch(cmd);
    _waitUntilStable();
    _sender.sendResponse(cmd, res == 0, sys);
    sys.resultCommand(res);
}
