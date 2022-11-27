/*
 * An oscilloscope program. View the output and adjust configs from Serial Plotter.
 * Please see serial_input_handler.ino for how to adjust configs
 * Maximum frequency for digital  readings is ~ 13.2 Mhz
 * Maximum frequency for analogue readings is ~ 57.5 Khz
 * Minimum frequency is 500 hz
 * 
 * This program will run for 35 minutes before crashing due to an integer overflow which I can't be bothered fixing. 
 * If you need to run this for longer then turn it off and on again.  
 * 
 * pin 16 has 258k ohms of resistance, 200 before and 58 after. resulting in 58/358 (0.2248062015503876) * the input voltage. 14.679 V -> 3.3 V
 */
#define PIN_OUT 11
#define PIN_IN 14
#define PIN_IN_5V 15
#define PIN_IN_12V 16

#define MODE_OSCILLOSCOPE 1
#define MODE_VOLTMETER 2
#define MODE_MEASURE_FREQUENCY 3

#define DIGITAL_READ_MICROS 0.075688
#define ANALOG_READ_MICROS 17.376194

int read_delay_micros = 18;
bool analogue = false;
int screen_delay_ms = 500;
int pin_in = PIN_IN_12V;
int mode = MODE_OSCILLOSCOPE;

void setup() {
  mode=MODE_OSCILLOSCOPE;
  screen_delay_ms=500;//2fps
  toggle_analogue();
}

void update_frequency(int frequency) {
  if(frequency < 500) return;
  float desired_micros_per_reading = 1000000.0/((float)frequency);
  float new_delay = desired_micros_per_reading - (analogue ? ANALOG_READ_MICROS : DIGITAL_READ_MICROS);
  read_delay_micros=round(new_delay);
  if(read_delay_micros < 0) read_delay_micros = 0;
}

void toggle_analogue() {
  if(analogue) {
    analogue=false;
    read_delay_micros +=18;
  } else {
    analogue=true;
    read_delay_micros -=18;
    if(read_delay_micros < 0) read_delay_micros = 0;
  }
}

float twelve_volt_calibration = 1.0/1023*14.679;
float five_volt_calibration = 1.0/1023*5.0;
float analog_to_voltage_calibration = 1.0/1023*3.3;

float convert_to_volts(int val) {
  switch(pin_in) {
    case PIN_IN_5V:
      return five_volt_calibration*((float)val);
    case PIN_IN_12V:
      return twelve_volt_calibration*((float)val);
    case PIN_IN :
    default:
      return analog_to_voltage_calibration*((float)val);
  }
}

int get_val() {
   return analogue ? analogRead(pin_in) : digitalRead(pin_in);
}

void sync_with_signal() {
  int now = micros();
  pinMode(pin_in,INPUT);
  while(digitalRead(pin_in)) 
    if(micros() - now > 1000) 
      break;
  while(!digitalRead(pin_in)) 
    if(micros() - now > 1000) 
      break;
  pinMode(pin_in,analogue ? INPUT_DISABLE : INPUT);
}


float get_time_to_wait() {
  switch(mode) {
    case MODE_OSCILLOSCOPE:
      return oscilloscope_time_to_wait();
    case MODE_VOLTMETER:
      return voltmeter_time_to_wait();
    case MODE_MEASURE_FREQUENCY:
      return measure_frequency_time_to_wait();
  }
  return 0;
}

void loop() {
  sync_with_signal();
  switch(mode) {
    case MODE_OSCILLOSCOPE:
      generate_oscilloscope_graph();
      break;
    case MODE_VOLTMETER:
      generate_voltmeter_output();
      break;
    case MODE_MEASURE_FREQUENCY:
      generate_signal_frequency_output();
      break;
  }
  handle_input();
  delay(get_time_to_wait());
}
