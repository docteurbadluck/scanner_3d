#include "ServoMotor_Driver.hpp"

#include <pico/stdlib.h>
#include <stdio.h>

namespace
{
static constexpr uint16_t IDLE_MAX_ADC = 100;

static uint32_t adc_to_mA(uint16_t counts)
{
    return (uint32_t)counts * 3300u / 2048u;
}

static bool isBaselineValid(uint16_t baseline)
{
    if (baseline <= IDLE_MAX_ADC)
        return true;
    printf("[servo] selfTest FAIL: ADC floating (%lumA)\n", adc_to_mA(baseline));
    return false;
}

static bool isCurrentDetected(uint16_t peak, uint16_t threshold)
{
    if (peak >= threshold)
        return true;
    printf("[servo] selfTest FAIL: %lumA detected (threshold %lumA)\n",
        adc_to_mA(peak), adc_to_mA(threshold));
    return false;
}
}

ServoSelfTestResult ServoMotor_Driver::selfTest()
{
    const uint16_t baseline = _readAdcPeak(50);
    if (!isBaselineValid(baseline))
        return ServoSelfTestResult::NO_CURRENT;
    const float    angle     = _angleForPos(Pos_hand::DOWN_A);
    _setPulse(_angleToPulse(angle));
    const uint16_t peak      = _readAdcPeak(_cfg.move_delay_ms);
    const uint16_t threshold = baseline + _cfg.current_threshold_adc;
    goInitialPos();
    if (!isCurrentDetected(peak, threshold))
        return ServoSelfTestResult::NO_CURRENT;
    return ServoSelfTestResult::OK;
}
