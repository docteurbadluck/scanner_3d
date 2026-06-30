#include "../ExecuteOrder_UC.hpp"

const ExecuteOrder_UC::DispatchEntry ExecuteOrder_UC::DISPATCH_TABLE[] = {
    {"TAKE_PHOTO",      &ExecuteOrder_UC::_handleTakePhoto},
    {"PING_PICO",       &ExecuteOrder_UC::_handlePingPico},
    {"GET_PICO_STATUS", &ExecuteOrder_UC::_handleGetPicoStatus},
    {"TEST_HARDWARE",   &ExecuteOrder_UC::_handleTestHardware},
    {"PLATE_NEXT",      &ExecuteOrder_UC::_handlePlateNext},
    {"POSITION_A",      &ExecuteOrder_UC::_handlePositionA},
    {"POSITION_B",      &ExecuteOrder_UC::_handlePositionB},
    {"POSITION_C",      &ExecuteOrder_UC::_handlePositionC},
    {"POSITION_D",      &ExecuteOrder_UC::_handlePositionD},
    {"INITIAL_POS",     &ExecuteOrder_UC::_handleInitialPos},
};

int ExecuteOrder_UC::_dispatch(const std::string &cmd)
{
    for (const DispatchEntry &entry : DISPATCH_TABLE)
        if (cmd == entry.cmd)
            return (this->*entry.handler)();
    return _isNoOp(cmd) ? 0 : -1;
}
