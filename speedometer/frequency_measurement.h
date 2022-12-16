#ifndef FREQUENCY_MEASUREMENT_H
#define FREQUENCY_MEASUREMENT_H
class FrequencyMeasurer {
	protected:
		uint8_t input_pin;
		bool starts_at_high;
		unsigned long micros_of_last_change;
		bool currently_high;
		float current_frequency;
		float current_gap_between_pulses;
		float maximum_gap_between_pulses;

		bool make_measurement();
		void set_frequency(unsigned long micros_gap);

	public:
    float minimum_frequency;
    
		FrequencyMeasurer(uint8_t input_pin, bool starts_at_high, float minimum_frequency);
		void measure_for_frequency_signal();
		float get_current_frequency_hz();
};

#endif
