#include "PlateController_UC.hpp"

PlateController_UC::PlateController_UC(IStepperMotor &stepper)
    : _stepper(stepper), _current_step(-1)
{}
