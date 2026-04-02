#include "ExecuteOrder_UC.hpp"

ExecuteOrder_UC::ExecuteOrder_UC(CaptureData_UC &capture, SendPhotoToComputer_UC &send,
                                 SendToComputer_UC &sender, IPico &pico)
    : _capture(capture), _send(send), _sender(sender), _pico(pico)
{}

int ExecuteOrder_UC::_dispatch(const std::string &cmd)
{
    if (cmd == "START_CAPTURE")   return (_capture.execute() && _send.execute()) ? 0 : 1;
    if (cmd == "GET_PICO_STATUS") { _sender.sendRaw(_pico.getPicoStatus()); return 0; }
    return 0;
}

void ExecuteOrder_UC::execute(System &sys)
{
    std::string cmd = sys.getCommandToExecute();
    int res = _dispatch(cmd);
    if (cmd != "GET_PICO_STATUS")
        _sender.sendResponse(cmd, res == 0, sys);
}
