#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_dispatchSystem(const std::string &cmd)
{
    if (cmd == "PLATE_NEXT")
        return _plate.rotateTo(_plate.getPos() + 1) ? 0 : 1;
    if (cmd == "INITIAL_POS")
        return _goInitialPos();
    if (cmd == "TEST_HARDWARE")
        return _execute_hardware_test();
    return 0;
}
