#include "ExecuteOrder_UC.hpp"

void ExecuteOrder_UC::execute(System &sys)
{
    std::string cmd = sys.getCommandToExecute();
    int res = _dispatch(cmd);
    if (res == -1)
        _sender.sendInvalidCmd();
    else if (cmd != "GET_PICO_STATUS")
        _sender.sendResponse(cmd, res == 0, sys);
}
