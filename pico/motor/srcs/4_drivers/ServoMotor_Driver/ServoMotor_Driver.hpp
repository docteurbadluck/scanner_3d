#pragma once

#include "3_interface/IServoMotor.hpp"

#include <hardware/pwm.h>
#include <cstdint>

struct ServoMotor_DriverConfig
{
    float    angle_up_a_deg   = 0.0f;
    float    angle_up_b_deg   = 45.0f;
    float    angle_down_a_deg = 90.0f;
    float    angle_down_b_deg = 135.0f;
    uint32_t move_delay_ms    = 400;
};

struct ServoMotor_DriverPins
{
    uint8_t pwm_pin = 6;
};

class ServoMotor_Driver : public IServoMotor
{
public:
    ServoMotor_Driver(const ServoMotor_DriverConfig &cfg,
                      const ServoMotor_DriverPins   &pins);
    ~ServoMotor_Driver() override = default;

    bool goInitialPos() override;
    bool goTo(Pos_hand pos) override;

private:
    ServoMotor_DriverConfig _cfg;
    ServoMotor_DriverPins   _pins;

    uint32_t _pwm_slice;
    uint32_t _pwm_channel;

    void                   _initPWM();
    void                   _setPulse(uint16_t pulse_us);
    [[nodiscard]] float    _angleForPos(Pos_hand pos) const;
    [[nodiscard]] uint16_t _angleToPulse(float angle_deg) const;
};
