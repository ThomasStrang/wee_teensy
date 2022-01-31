/*
 * An oscilloscope program. 
 * Please see serial_input_handler.ino for adjusting configs
 * Maximum frequency for digital readings is ~2.6 Mhz
 * Maximum frequency for analogue readings is ~55 Khz
 */
#define PIN_OUT 11
#define PIN_IN 15
#define PIN_IN_5V 16
#define PIN_IN_12V 17

IntervalTimer update_pwm_width_timer;


#define DIGITAL_READ_MICROS 0.379
#define ANALOG_READ_MICROS 17.94

int delay_micros = 18;
bool analog = false;
int pixels_per_reading = 1;
int screen_delay_ms = 250;
int pin_in = PIN_IN;

void print(String str, float i) {
  Serial.print(str);
  Serial.println(i);
}

void setup() {
  // put your setup code here, to run once:
  analogWrite(PIN_OUT,10);
  update_pwm_width_timer.begin(update_pwm_width, 50000);
}

int pwm_width=16;

void update_pwm_width() {
  pwm_width+=1;
  if(pwm_width >= 256) pwm_width = 0;
  analogWrite(PIN_OUT,pwm_width);
}

void update_frequency(int frequency) {
  float desired_micros_per_reading = 1000000.0/((float)frequency);
  float new_delay = desired_micros_per_reading - (analog ? ANALOG_READ_MICROS : DIGITAL_READ_MICROS);
  delay_micros=round(new_delay);
  if(delay_micros < 0) delay_micros = 0;
}

void toggle_analog() {
  if(analog) {
    analog=false;
    delay_micros +=18;
  } else {
    analog=true;
    delay_micros -=18;
    if(delay_micros < 0) delay_micros = 0;
  }
}

float analog_to_voltage_calibration = 1.0/1024*3.3;

float convert_to_volts(int val) {
  return analog_to_voltage_calibration*val;
}

void sync_with_signal() {
  int now = micros();
  while(digitalRead(pin_in)) 
    if(micros() - now > 1000) 
      break;
  while(!digitalRead(pin_in)) 
    if(micros() - now > 1000) 
      break;
}


void print_vals() {
  for(int i = 0; i < 500; i++) {
    if(!(i%pixels_per_reading)) {
      int val = get_val();
      for(int j = 0; j < pixels_per_reading && j+i<500; j++) {
        Serial.println(val);
      }
      delayMicroseconds(delay_micros);
    }
  }
}

int get_val() {
   return analog ? analogRead(pin_in) : digitalRead(pin_in);
}

void calculate_frequency() {
  analog=false;
  delay_micros=0;
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
  delay(screen_delay_ms-(delay_micros>>1));
  handle_input();
}
