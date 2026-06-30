#include "../MotorDC_Driver.hpp"

uint16_t MotorDC_Driver::_measureBaseline() const
{
	static constexpr uint8_t SAMPLES = 5;
	uint32_t sum = 0;
	for (uint8_t i = 0; i < SAMPLES; ++i)
		sum += _io.read_raw_adc();
	return static_cast<uint16_t>(sum / SAMPLES);
}
