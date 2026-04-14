#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_dispatch(const std::string &cmd)
{
    if (cmd == "START_CAPTURE")
        return (_capture.execute() && _send.execute()) ? 0 : 1;
    if (cmd == "PING_PICO")
    {
        _sender.sendRaw(_pico.pingPico());
        return 0;
    }
    if (cmd == "GET_PICO_STATUS")
    {
        std::string res = _pico.getPicoStatus();
       printf("%s\n", res.c_str());
        _sender.sendPicoStatus(res);
        return 0;
    }
    if (_isNoOp(cmd))
        return 0;
    return -1;
}
