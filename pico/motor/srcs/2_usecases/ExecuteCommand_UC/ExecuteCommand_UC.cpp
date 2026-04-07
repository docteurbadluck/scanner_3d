#include "ExecuteCommand_UC.hpp"

ExecuteCommand_UC::ExecuteCommand_UC(ArmController_UC &arm, HandController_UC &hand,
                                     PlateController_UC &plate, VibrationMonitor_UC &vibration,
                                     SendToPi_UC &sender)
    : _arm(arm), _hand(hand), _plate(plate), _vibration(vibration), _sender(sender)
{}
