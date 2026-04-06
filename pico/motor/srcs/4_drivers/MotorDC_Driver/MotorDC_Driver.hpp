#pragma once

#include "3_interface/IMotorDC.hpp"

#include <cstdint>
#include <functional>

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

struct MotorDC_DriverIO
{
    std::function<void(uint8_t)>  drive_up;
    std::function<void(uint8_t)>  drive_down;
    std::function<void()>         stop;
    std::function<bool()>         is_up_pressed;
    std::function<bool()>         is_down_pressed;
    std::function<uint32_t()>     now_ms;
    std::function<void(uint32_t)> sleep_ms;
};

class MotorDC_Driver : public IMotorDC
{
private:
    MotorDC_DriverConfig _cfg;
    MotorDC_DriverIO     _io;

    bool _wait_until_pressed(Pos pos) const;

public:
    MotorDC_Driver(const MotorDC_DriverConfig &cfg, const MotorDC_DriverPins &pins);
    MotorDC_Driver(const MotorDC_DriverConfig &cfg, MotorDC_DriverIO io);

    ~MotorDC_Driver() override = default;

    bool goInitialPos() override;
    bool goTo(Pos pos) override;
};
