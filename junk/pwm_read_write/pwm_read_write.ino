/*
 * An oscilloscope program. View the output and adjust configs from Serial Plotter.
 * Please see serial_input_handler.ino for how to adjust configs
 * Maximum frequency for digital  readings is ~ 13.2 Mhz
 * Maximum frequency for analogue readings is ~ 57.5 Khz
 * Minimum frequency is 500 hz
 * 
 * pin 16 has 258k ohms of resistance, 200 before and 58 after. resulting in 58/358 (0.2248062015503876) * the input voltage. 14.679 V -> 3.3 V
 */
#define PIN_OUT 11
#define PIN_IN 14
#define PIN_IN_5V 15
#define PIN_IN_12V 16

IntervalTimer update_pwm_width_timer;


#define DIGITAL_READ_MICROS 0.075688
#define ANALOG_READ_MICROS 17.376194

int read_delay_micros = 18;
bool analog = false;
int pixels_per_reading = 1;
int screen_delay_ms = 250;
int pin_in = PIN_IN_12V;

void print(String str, float i) {
  Serial.print(str);
  Serial.println(i);
}

void setup() {
  analogWrite(PIN_OUT,128);
  update_pwm_width_timer.begin(update_pwm_width, 200000);
  toggle_analog();
}

int pwm_width=16;

void update_pwm_width() {
  pwm_width+=4;
  if(pwm_width >= 256) pwm_width = 0;
  analogWrite(PIN_OUT,pwm_width);
}

void update_frequency(int frequency) {
  if(frequency < 500) return;
  float desired_micros_per_reading = 1000000.0/((float)frequency);
  float new_delay = desired_micros_per_reading - (analog ? ANALOG_READ_MICROS : DIGITAL_READ_MICROS);
  read_delay_micros=round(new_delay);
  if(read_delay_micros < 0) read_delay_micros = 0;
}

void toggle_analog() {
  if(analog) {
    analog=false;
    read_delay_micros +=18;
  } else {
    analog=true;
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

void sync_with_signal() {
  int now = micros();
  pinMode(pin_in,INPUT);
  while(digitalRead(pin_in)) 
    if(micros() - now > 1000) 
      break;
  while(!digitalRead(pin_in)) 
    if(micros() - now > 1000) 
      break;
  pinMode(pin_in,analog ? INPUT_DISABLE : INPUT);
}

void send_to_serial(int* vals, int num) {
  for(int i = 0; i < num; i++) {
    if(analog) {
      Serial.println(convert_to_volts(vals[i]));
    } else {
      Serial.println(vals[i]);
    }
  }
}

void print_vals() {
  int vals[500];
  for(int i = 0; i < 500; i++) {
    if(!(i%pixels_per_reading)) {
      int val = get_val();
      for(int j = 0; j < pixels_per_reading && j+i<500; j++) {
        vals[i]=val;
      }
      delayMicroseconds(read_delay_micros);
    }
  }
  send_to_serial(vals,500);
}

int get_val() {
   return analog ? analogRead(pin_in) : digitalRead(pin_in);
}

//not used in prod program , useful for debugging.
void calculate_frequency() {
  analog=false;
  read_delay_micros=0;
  int total_readings = -500;
  int end = micros()+10000000;//10 seconds from now
  while(int(micros()) < end) {
    total_readings += 500;
    print_vals();
  }
  int frequency_in_hz=total_readings/10;
  while(true) {
    Serial.println(frequency_in_hz);
    delay(1000);
  }
}

void loop() {
  sync_with_signal();
//  calculate_frequency();
  print_vals();
  int time_to_read = read_delay_micros>>1;//using micros in a millisecond context multiplies the time by 1000. bitshifting by 1 divides by 2, resulting in total multiplication of 500.
  int time_to_wait = screen_delay_ms - time_to_read;
  if(time_to_wait > 0) {
    delay(time_to_wait);//technically this doesn't take into account the time to display (or handling input), but it's only ~0.1 milliseconds so it doesn't really matter.
  }
  handle_input();
}
