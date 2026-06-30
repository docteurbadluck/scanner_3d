#include "../MotorDC_Driver.hpp"

bool MotorDC_Driver::_isStalled(uint8_t &overThresholdCount) const
{
	const uint16_t raw = _io.read_raw_adc();
	const uint16_t deviation = raw > _zeroBaseline ? raw - _zeroBaseline : _zeroBaseline - raw;
	if (deviation <= _cfg.stall_threshold_adc)
	{
		overThresholdCount = 0;
		return false;
	}
	overThresholdCount++;
	return overThresholdCount >= _cfg.stall_debounce_polls;
}
