#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handlePosition(const std::string &armPose, const std::string &camPose)
{
    return (_pico.setArmPosition(armPose) && _pico.setCameraPosition(camPose)) ? 0 : 1;
}
