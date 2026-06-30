#include "0_orchestration/config.hpp"

#include <pico/stdlib.h>
#include <pico/time.h>
#include <hardware/pwm.h>
#include <hardware/adc.h>

namespace
{
struct PwmPins { uint32_t s1, c1, s2, c2; };

void init_pwm_channel(uint8_t pin, uint32_t &slice, uint32_t &channel)
{
	gpio_set_function(pin, GPIO_FUNC_PWM);
	slice   = pwm_gpio_to_slice_num(pin);
	channel = pwm_gpio_to_channel(pin);
}

PwmPins init_motor_pwm(const MotorDC_DriverPins &pins)
{
	PwmPins pw;
	init_pwm_channel(pins.in1_pin, pw.s1, pw.c1);
	init_pwm_channel(pins.in2_pin, pw.s2, pw.c2);
	pwm_set_wrap(pw.s1, 1000);
	pwm_set_wrap(pw.s2, 1000);
	pwm_set_enabled(pw.s1, true);
	pwm_set_enabled(pw.s2, true);
	return pw;
}

void init_buttons(const MotorDC_DriverPins &pins)
{
	gpio_init(pins.btn_up_pin);
	gpio_set_dir(pins.btn_up_pin, GPIO_IN);
	gpio_pull_up(pins.btn_up_pin);
	gpio_init(pins.btn_down_pin);
	gpio_set_dir(pins.btn_down_pin, GPIO_IN);
	gpio_pull_up(pins.btn_down_pin);
}

bool isPressed(uint8_t pin)
{
	return !gpio_get(pin);
}

void initAcs712(uint8_t pin)
{
	adc_init();
	adc_gpio_init(pin);
}

uint16_t readAcs712(uint8_t channel)
{
	adc_select_input(channel);
	return adc_read();
}

void drive(const PwmPins &pw, bool up, uint8_t speed_percent)
{
	const uint16_t level = static_cast<uint16_t>(speed_percent * 10U);
	pwm_set_chan_level(up ? pw.s2 : pw.s1, up ? pw.c2 : pw.c1, 0);
	pwm_set_chan_level(up ? pw.s1 : pw.s2, up ? pw.c1 : pw.c2, level);
}

void stop(const PwmPins &pw)
{
	pwm_set_chan_level(pw.s1, pw.c1, 0);
	pwm_set_chan_level(pw.s2, pw.c2, 0);
}

void printRestSamples(uint8_t channel)
{
	for (int i = 0; i < 10; i++)
	{
		printf("REST adc=%u\n", readAcs712(channel));
		sleep_ms(200);
	}
}

bool shouldStop(bool up, const MotorDC_DriverPins &pins)
{
	return isPressed(up ? pins.btn_up_pin : pins.btn_down_pin);
}

void runDirection(const PwmPins &pw, const MotorDC_DriverPins &pins, uint8_t channel,
                   bool up, uint8_t speed_percent, uint32_t duration_ms)
{
	drive(pw, up, speed_percent);
	const uint32_t start = to_ms_since_boot(get_absolute_time());
	while (to_ms_since_boot(get_absolute_time()) - start < duration_ms)
	{
		printf("%s adc=%u\n", up ? "UP" : "DOWN", readAcs712(channel));
		if (shouldStop(up, pins))
			break;
		sleep_ms(200);
	}
	stop(pw);
}
}

int main()
{
	stdio_init_all();
	sleep_ms(2000);
	const PwmPins pw = init_motor_pwm(MOTOR_DC_PINS);
	init_buttons(MOTOR_DC_PINS);
	initAcs712(MOTOR_DC_PINS.acs712_pin);
	const uint8_t channel = MOTOR_DC_PINS.acs712_pin - 26u;

	printRestSamples(channel);
	runDirection(pw, MOTOR_DC_PINS, channel, true, MOTOR_DC_CONFIG.speed_percent, 3000);
	runDirection(pw, MOTOR_DC_PINS, channel, false, MOTOR_DC_CONFIG.speed_percent, 3000);

	while (true)
		tight_loop_contents();
}
