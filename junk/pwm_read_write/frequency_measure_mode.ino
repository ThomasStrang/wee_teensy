/*
 * In frequency measure mode either of 2 things are displayed
  - the chart of a digital signal frequency over time (serial plotter)
  - the current frequency of a digital signal (serial monitor)
  - the average frequency, the variance in frequency, max and min frequencies, over a given number of signals measured (default 1) (serial monitor)

  */

unsigned int number_of_signals_to_measure=1;
unsigned int measure_signal_timeout_micros=1000000;
bool measure_front_of_signal=true;

unsigned int micros_spent_measuring_frequency=0;//This will be set every time the frequency is measured.

unsigned int measure_signal_strength() {
  unsigned int strt=micros();
  unsigned int highest=get_val();
  while(micros() - strt < measure_signal_timeout_micros) {
    unsigned int aval=get_val();
    if(aval>highest)
      highest=aval;
  }
  return highest;
}

bool signal_is_high(int threshold) {
  return get_val()>threshold;
}

int read_signal_start_return_micros(unsigned int high_threshold) {

  if(measure_front_of_signal) {
    while(signal_is_high(high_threshold)){
      //TODO add timeouts.
    }
    while(!signal_is_high(high_threshold)){
      //TODO add timeouts.
    }
    //return the exact time that the signal goes high, the "front" of the signal or the "left" of the signal
    return micros();
  } else {
    while(!signal_is_high(high_threshold)){
      //TODO add timeouts.
    }
    while(signal_is_high(high_threshold)){
      //TODO add timeouts.
    }
    //return the exact time that the signal goes low, the "rear" of the signal or the "right" of the signal
    return micros();
  }
}

float micros_taken_to_frequency_hz(int micros_taken) {
  return 1000000.0/micros_taken;
}

void generate_signal_frequency_output() {
  unsigned int strt=micros();
  unsigned int high_threshold = 0;
  if(analogue) {
    high_threshold = measure_signal_strength()>>1;
  }
  unsigned int first_signal_micros = read_signal_start_return_micros(high_threshold);
  unsigned int prev_signal_micros  = first_signal_micros;
  unsigned int least_signal_micros_taken = ((unsigned int)0)-1;
  unsigned int most_signal_micros_taken = 0;
  for(int i = 0; i < number_of_signals_to_measure; i++) {
    unsigned int signal_micros = read_signal_start_return_micros(high_threshold);
    unsigned int signal_micros_taken = signal_micros - prev_signal_micros;
    if(signal_micros_taken < least_signal_micros_taken) least_signal_micros_taken=signal_micros_taken;
    if(signal_micros_taken > most_signal_micros_taken) most_signal_micros_taken=signal_micros_taken;
    prev_signal_micros = signal_micros;
  }
  unsigned int average_micros_taken = (micros()-first_signal_micros)/number_of_signals_to_measure;
  send_to_serial_monitor("\nFrequency Measurement Complete.\nSignal Voltage Strength chosen: ", convert_to_volts(high_threshold<<1));
  send_to_serial_monitor("Signal Voltage threshold chosen: ", convert_to_volts(high_threshold));
  send_to_serial_monitor(micros_taken_to_frequency_hz(average_micros_taken)," Hz - Average");
  send_to_serial_monitor(micros_taken_to_frequency_hz(most_signal_micros_taken)," Hz - Minimum");
  send_to_serial_monitor(micros_taken_to_frequency_hz(least_signal_micros_taken)," Hz - Maximum");
  micros_spent_measuring_frequency = micros() - strt;
}


float measure_frequency_time_to_wait() {
  int time_to_wait = screen_delay_ms - (micros_spent_measuring_frequency*1000);
  return time_to_wait>=0?time_to_wait:0;
}
