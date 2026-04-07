#include "StepperMotor_Driver.hpp"

#include <hardware/gpio.h>
#include <pico/stdlib.h>

StepperMotor_Driver::StepperMotor_Driver(const StepperMotor_DriverConfig &cfg,
                                         const StepperMotor_DriverPins   &pins)
    : _cfg(cfg), _pins(pins)
{
    _initGPIO();
}

void StepperMotor_Driver::_initGPIO()
{
    gpio_init(_pins.step_pin);
    gpio_set_dir(_pins.step_pin, GPIO_OUT);
    gpio_put(_pins.step_pin, false);

    gpio_init(_pins.dir_pin);
    gpio_set_dir(_pins.dir_pin, GPIO_OUT);
    gpio_put(_pins.dir_pin, false);

    if (_pins.has_enable)
    {
        gpio_init(_pins.enable_pin);
        gpio_set_dir(_pins.enable_pin, GPIO_OUT);
        gpio_put(_pins.enable_pin, true);
    }
}

void StepperMotor_Driver::_enable()
{
    if (_pins.has_enable)
        gpio_put(_pins.enable_pin, false);
}

void StepperMotor_Driver::_pulseStep()
{
    gpio_put(_pins.step_pin, true);
    ::sleep_us(_cfg.step_pulse_us);
    gpio_put(_pins.step_pin, false);
}

bool StepperMotor_Driver::goInitialPos()
{
    _enable();
    gpio_put(_pins.dir_pin, true);
    return true;
}

bool StepperMotor_Driver::stepForward()
{
    gpio_put(_pins.dir_pin, true);
    ::sleep_us(_cfg.dir_setup_us);
    _pulseStep();
    ::sleep_us(_cfg.step_delay_us);
    return true;
}
