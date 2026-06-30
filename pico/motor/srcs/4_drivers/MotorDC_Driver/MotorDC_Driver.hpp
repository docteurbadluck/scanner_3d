#pragma once

#include "3_interface/IMotorDC.hpp"

#include <cstdint>
#include <functional>

struct MotorDC_DriverConfig
{
    uint8_t  speed_percent        = 40;
    uint32_t timeout_ms           = 8000;
    uint32_t poll_interval_ms     = 5;
    uint16_t stall_threshold_adc  = 500;
    uint8_t  stall_debounce_polls = 1;
    uint32_t stall_grace_ms       = 0;
};

struct MotorDC_DriverPins
{
    uint8_t in1_pin            = 0;
    uint8_t in2_pin            = 1;
    uint8_t btn_up_pin         = 2;
    uint8_t btn_down_pin       = 3;
    bool    buttons_active_low = true;
    uint8_t acs712_pin         = 28;
};

enum class WaitResult
{
    REACHED,
    TIMEOUT,
    STALLED,
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
    std::function<uint16_t()>     read_raw_adc;
};

class MotorDC_Driver : public IMotorDC
{
private:
    MotorDC_DriverConfig _cfg;
    MotorDC_DriverIO     _io;
    uint8_t              _adc_channel = 0;
    uint16_t             _zeroBaseline = 0;

    void       _initADC(uint8_t acs712_pin);
    bool       _isTargetPressed(Pos pos) const;
    bool       _isStalled(uint8_t &overThresholdCount) const;
    WaitResult _wait_until_pressed(Pos pos) const;
    uint16_t   _measureBaseline() const;
    WaitResult _goUp();
    WaitResult _goDown();

public:
    MotorDC_Driver(const MotorDC_DriverConfig &cfg, const MotorDC_DriverPins &pins);
    MotorDC_Driver(const MotorDC_DriverConfig &cfg, MotorDC_DriverIO io);

    ~MotorDC_Driver() override = default;

    void           stop() override;
    bool           goInitialPos() override;
    bool           goTo(Pos pos) override;
    SelfTestResult selfTest() override;
};
