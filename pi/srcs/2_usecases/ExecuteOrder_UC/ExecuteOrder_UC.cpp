#include "ExecuteOrder_UC.hpp"
#include "1_domain/JsonMessage/JsonMessage.hpp"

ExecuteOrder_UC::ExecuteOrder_UC(CaptureData_UC &capture, SendPhotoToComputer_UC &send,
                                 SendToComputer_UC &sender, IPico &pico)
    : _capture(capture), _send(send), _sender(sender), _pico(pico)
{}

int ExecuteOrder_UC::_dispatch(const std::string &cmd)
{
    if (cmd == "START_CAPTURE")   return (_capture.execute() && _send.execute()) ? 0 : 1;
    if (cmd == "GET_PICO_STATUS")
    {
        _sender.sendRaw(_pico.getPicoStatus());
        return 0;
    }
    if (cmd == "PING")       return 0;
    if (cmd == "GET_STATUS") return 0;
    if (cmd == "PONG")       return 0;
    return -1;
}

void ExecuteOrder_UC::execute(System &sys)
{
    std::string cmd = sys.getCommandToExecute();
    int res = _dispatch(cmd);
    if (res == -1)
        _sender.sendInvalidCmd();
    else if (cmd != "GET_PICO_STATUS")
        _sender.sendResponse(cmd, res == 0, sys);
}
