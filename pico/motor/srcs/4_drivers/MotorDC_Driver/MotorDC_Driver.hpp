#pragma once

#include "3_interface/IMotorDC.hpp"

#include <cstdint>

struct MotorDC_DriverConfig
{
    uint8_t  speed_percent = 60; // fixed once at construction
    uint32_t timeout_ms = 3000;
    uint32_t poll_interval_ms = 5;
};

struct MotorDC_DriverPins
{
    uint8_t in1_pin = 0;
    uint8_t in2_pin = 1;
    uint8_t btn_up_pin = 2;
    uint8_t btn_down_pin = 3;

    // true for endstops wired with pull-up and active low (common case)
    bool    buttons_active_low = true;
};

class MotorDC_Driver : public IMotorDC
{
private:
    MotorDC_DriverConfig _cfg;
    MotorDC_DriverPins   _pins;

    uint32_t _in1_pwm_slice;
    uint32_t _in1_pwm_channel;
    uint32_t _in2_pwm_slice;
    uint32_t _in2_pwm_channel;

    void _drive_up();
    void _drive_down();
    void _stop();
    bool _is_up_pressed() const;
    bool _is_down_pressed() const;

    bool _wait_until_pressed(Pos pos) const;

public:
    MotorDC_Driver(const MotorDC_DriverConfig &cfg, const MotorDC_DriverPins &pins);

    ~MotorDC_Driver() override = default;

    bool goInitialPos() override;
    bool goTo(Pos pos) override;
};
