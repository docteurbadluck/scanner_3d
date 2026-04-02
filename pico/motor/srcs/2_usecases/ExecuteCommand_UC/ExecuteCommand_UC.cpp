#include "ExecuteCommand_UC.hpp"

ExecuteCommand_UC::ExecuteCommand_UC(ArmController_UC &arm, HandController_UC &hand,
                                     PlateController_UC &plate, VibrationMonitor_UC &vibration,
                                     SendToPi_UC &sender)
    : _arm(arm), _hand(hand), _plate(plate), _vibration(vibration), _sender(sender)
{}

int ExecuteCommand_UC::_goInitialPos()
{
    bool ok = _arm.joinInitialPos();
    ok = _hand.joinInitialPos() && ok;
    ok = _plate.joinInitialPos() && ok;
    return ok ? 0 : 1;
}

int ExecuteCommand_UC::_dispatch(const std::string &cmd)
{
    if (cmd == "ARM_UP")      return _arm.joinPos(Pos::UP) ? 0 : 1;
    if (cmd == "ARM_DOWN")    return _arm.joinPos(Pos::DOWN) ? 0 : 1;
    if (cmd == "CAM_A")       return _hand.joinPos(Pos_hand::UP_A) ? 0 : 1;
    if (cmd == "CAM_B")       return _hand.joinPos(Pos_hand::UP_B) ? 0 : 1;
    if (cmd == "CAM_C")       return _hand.joinPos(Pos_hand::DOWN_A) ? 0 : 1;
    if (cmd == "CAM_D")       return _hand.joinPos(Pos_hand::DOWN_B) ? 0 : 1;
    if (cmd == "PLATE_NEXT")  return _plate.rotateTo(_plate.getPos() + 1) ? 0 : 1;
    if (cmd == "INITIAL_POS") return _goInitialPos();
    return 0;
}

void ExecuteCommand_UC::_waitUntilStable()
{
    while (!_vibration.isStable()) {}
}

void ExecuteCommand_UC::execute(System &sys)
{
    std::string cmd = sys.getCommandToExecute();
    int res = _dispatch(cmd);
    _waitUntilStable();
    _sender.sendResponse(cmd, res == 0, sys);
    sys.resultCommand(res);
}
