#include "ExecuteCommand_UC.hpp"

ExecuteCommand_UC::ExecuteCommand_UC(ArmController_UC &arm, HandController_UC &hand,
                                     PlateController_UC &plate,
                                     SendToPi_UC &sender, TestHardware_UC &testHardware)
    : _arm(arm), _hand(hand), _plate(plate),
      _sender(sender), _testHardware(testHardware)
{}
