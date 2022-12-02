#include <Arduino.h>
#include "frequency_measurement.h"
FrequencyMeasurer::FrequencyMeasurer(uint8_t input_pin, bool starts_at_high, float minimum_frequency) : input_pin{input_pin}, starts_at_high{starts_at_high}, minimum_frequency{minimum_frequency}, micros_of_last_change{micros()}, current_frequency{minimum_frequency} {
	pinMode(input_pin,INPUT);
	currently_high=digitalRead(input_pin);
	maximum_gap_between_pulses=1000000.0/minimum_frequency;
	current_gap_between_pulses=maximum_gap_between_pulses;
}

bool FrequencyMeasurer::make_measurement() {
	return digitalRead(input_pin);
}

void FrequencyMeasurer::set_frequency(unsigned long micros_gap_since_last_pulse) {
	current_gap_between_pulses=micros_gap_since_last_pulse;
	current_frequency=current_gap_between_pulses/1000000.0;
}

void FrequencyMeasurer::measure_for_frequency_signal() {
	bool voltage=make_measurement();
	unsigned long now=micros();
	if(voltage!=currently_high) {
		if(voltage==starts_at_high) {
			//this is considered a pulse of the signal. if starts_at_high and voltage is high and currently_high is false then that means it just went high.
			set_frequency(now-micros_of_last_change);
			micros_of_last_change=now;
		}
		currently_high=voltage;
	}
	if(current_frequency!=minimum_frequency){
		unsigned long gap=now-micros_of_last_change;
		if(gap>maximum_gap_between_pulses) {
				set_frequency(maximum_gap_between_pulses);
		} else {
			if(gap>current_gap_between_pulses) {
				set_frequency(gap);
			}
		}
	}
}

float FrequencyMeasurer::get_current_frequency_hz() {
	return current_frequency;
}