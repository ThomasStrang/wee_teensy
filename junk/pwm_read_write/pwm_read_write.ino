#define PIN_OUT 11
#define PIN_IN 15
#define PIN_IN_5V 16
#define PIN_IN_12V 17

IntervalTimer update_pwm_width_timer;

int delay_micros = 1;
bool analog = false;
int pixels_per_reading = 1;
int screen_delay_ms = 250;
int pin_in = PIN_IN;

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

float digital_to_voltage_calibration = 1.0/1024*3.3;

float convert_to_volts(int val) {
  return digital_to_voltage_calibration*val;
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

void loop() {
  sync_with_signal();
  print_vals();
  delay(screen_delay_ms);
  handle_input();
}
