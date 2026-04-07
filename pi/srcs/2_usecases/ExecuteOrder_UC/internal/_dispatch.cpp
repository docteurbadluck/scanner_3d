#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_dispatch(const std::string &cmd)
{
    if (cmd == "START_CAPTURE")
        return (_capture.execute() && _send.execute()) ? 0 : 1;
    if (cmd == "GET_PICO_STATUS")
    {
        _sender.sendRaw(_pico.getPicoStatus());
        return 0;
    }
    if (_isNoOp(cmd))
        return 0;
    return -1;
}
