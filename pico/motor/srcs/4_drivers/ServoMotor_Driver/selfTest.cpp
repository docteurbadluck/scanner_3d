#include "ServoMotor_Driver.hpp"

#include <pico/stdlib.h>
#include <stdio.h>

namespace
{
static constexpr uint16_t IDLE_MAX_ADC = 100;

static uint32_t adc_to_mA(uint16_t counts, float shunt_ohms)
{
    return (uint32_t)(counts * 3300.0f / (4096.0f * shunt_ohms));
}

static bool isBaselineValid(uint16_t baseline, float shunt_ohms)
{
    if (baseline <= IDLE_MAX_ADC)
        return true;
    printf("[servo] selfTest FAIL: ADC floating (%lumA)\n", adc_to_mA(baseline, shunt_ohms));
    return false;
}

static bool isCurrentDetected(uint16_t peak, uint16_t threshold, float shunt_ohms)
{
    if (peak >= threshold)
        return true;
    printf("[servo] selfTest FAIL: %lumA detected (threshold %lumA)\n",
        adc_to_mA(peak, shunt_ohms), adc_to_mA(threshold, shunt_ohms));
    return false;
}
}

ServoSelfTestResult ServoMotor_Driver::selfTest()
{
    const float    shunt    = _cfg.shunt_ohms;
    const uint16_t baseline = _readAdcPeak(50);
    if (!isBaselineValid(baseline, shunt))
        return ServoSelfTestResult::NO_CURRENT;
    _setPulse(_angleToPulse(_angleForPos(Pos_hand::DOWN_A)));
    const uint16_t peak      = _readAdcPeak(_cfg.move_delay_ms);
    const uint16_t threshold = baseline + _cfg.current_threshold_adc;
    goInitialPos();
    if (!isCurrentDetected(peak, threshold, shunt))
        return ServoSelfTestResult::NO_CURRENT;
    return ServoSelfTestResult::OK;
}
