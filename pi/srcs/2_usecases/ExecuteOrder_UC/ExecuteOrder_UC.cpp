#include "ExecuteOrder_UC.hpp"

ExecuteOrder_UC::ExecuteOrder_UC(CaptureData_UC &capture, SendPhotoToComputer_UC &send,
                                 SendToComputer_UC &sender, IPico &pico)
    : _capture(capture), _send(send), _sender(sender), _pico(pico)
{}
