#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_dispatch(const std::string &cmd)
{
    if (cmd == "START_CAPTURE")   return _handleStartCapture();
    if (cmd == "PING_PICO")       return _handlePingPico();
    if (cmd == "GET_PICO_STATUS") return _handleGetPicoStatus();
    if (_isNoOp(cmd))             return 0;
    return -1;
}
