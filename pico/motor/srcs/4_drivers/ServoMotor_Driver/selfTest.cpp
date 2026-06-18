#include "ServoMotor_Driver.hpp"

#include <pico/stdlib.h>

namespace
{
static constexpr uint16_t IDLE_MAX_ADC = 100;

static bool isBaselineValid(uint16_t baseline)
{
    return baseline <= IDLE_MAX_ADC;
}

static bool isCurrentDetected(uint16_t peak, uint16_t threshold)
{
    return peak >= threshold;
}
}

ServoSelfTestResult ServoMotor_Driver::selfTest()
{
    const uint16_t baseline  = _readAdcPeak(50);
    if (!isBaselineValid(baseline))
        return ServoSelfTestResult::NO_CURRENT;
    _setPulse(_angleToPulse(_angleForPos(Pos_hand::DOWN_A)));
    const uint16_t peak      = _readAdcPeak(_cfg.move_delay_ms);
    const uint16_t threshold = baseline + _cfg.current_threshold_adc;
    goInitialPos();
    if (!isCurrentDetected(peak, threshold))
        return ServoSelfTestResult::NO_CURRENT;
    return ServoSelfTestResult::OK;
}
