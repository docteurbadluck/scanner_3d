#pragma once

#include "3_interface/IStepperMotor.hpp"

#include <cstdint>

struct StepperMotor_DriverConfig
{
    uint32_t dir_setup_us  = 10;
    uint32_t step_pulse_us = 10;
    uint32_t step_delay_us = 500;
};

struct StepperMotor_DriverPins
{
    uint8_t step_pin   = 10;
    uint8_t dir_pin    = 11;
    uint8_t enable_pin = 12;
    bool    has_enable = true;
};

class StepperMotor_Driver : public IStepperMotor
{
public:
    StepperMotor_Driver(const StepperMotor_DriverConfig &cfg,
                        const StepperMotor_DriverPins   &pins);
    ~StepperMotor_Driver() override = default;

    bool goInitialPos() override;
    bool stepForward() override;

private:
    StepperMotor_DriverConfig _cfg;
    StepperMotor_DriverPins   _pins;

    void _initGPIO();
    void _enable();
    void _pulseStep();
};
