//first char is the control char, the rest is an integer.
#define UPDATE_FREQUENCY 'F'
#define UPDATE_READINGS 'R'
#define UPDATE_FPS 'S'

#define TOGGLE_ANALOG 'A'
#define THREE_VOLTS '3'
#define FIVE_VOLTS '5'
#define TWELVE_VOLTS '2'

int read_int_from_serial() {
  int val=0;
  while(Serial.available()) {
    byte b = Serial.read();
    if(b == 10) break;
    val = val*10;
    val += b - '0';
  }
  return val;
}

void update_readings() {
  int val = read_int_from_serial();
  if(val>0 && val <=500) 
    pixels_per_reading=500/val;
}

void update_frequency() {
  update_frequency(read_int_from_serial());
}
void update_fps() {
  screen_delay_ms=1000/read_int_from_serial();
}

void update_voltage_mode() {
  int val = read_int_from_serial();
  if(val>0 && val <=500) 
    pixels_per_reading=500/val;
  
}

void handle_input() {
  if(Serial.available()) {
    switch(Serial.read()) {
      case UPDATE_FREQUENCY :
        update_frequency();
        break;
      case UPDATE_READINGS :
        update_readings();
        break;
      case UPDATE_FPS :
        update_fps();
        break;
      case TOGGLE_ANALOG :
        toggle_analog();
        break;
      case THREE_VOLTS :
        pin_in=PIN_IN;
        break;
      case FIVE_VOLTS :
        pin_in=PIN_IN_5V;
        break;
      case TWELVE_VOLTS :
        pin_in=PIN_IN_12V;
        break;        
    }
    while(Serial.available()) Serial.read();
  }
}
