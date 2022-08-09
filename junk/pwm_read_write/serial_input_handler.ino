//first char is the control char, the rest is an integer.
//setting too high a frequency will just cause it to run at max speed possible
#define UPDATE_FREQUENCY 'F'
#define UPDATE_FPS 'S'
#define TOGGLE_ANALOG 'A'
#define SET_MODE_VOLTMETER 'V'
#define SET_MODE_OSCILLOSCOPE 'O'
#define SET_MODE_MEASURE_FREQUENCY 'M'

//#define THREE_VOLTS '3'
//#define FIVE_VOLTS '5'
//#define TWELVE_VOLTS '2'

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

void handle_input() {
  if(Serial.available()) {
    switch(Serial.read()) {
      case UPDATE_FREQUENCY :
        update_frequency(read_int_from_serial());
        break;
      case UPDATE_FPS :
        screen_delay_ms=1000/read_int_from_serial();
        break;
      case TOGGLE_ANALOG :
        toggle_analogue();
        break;
      case SET_MODE_VOLTMETER :
        mode=MODE_VOLTMETER;
        break;
      case SET_MODE_OSCILLOSCOPE :
        mode=MODE_OSCILLOSCOPE;
        break;
      case SET_MODE_MEASURE_FREQUENCY :
        mode=MODE_MEASURE_FREQUENCY;
        break;
//      case THREE_VOLTS :
//        pin_in=PIN_IN;
//        break;
//      case FIVE_VOLTS :
//        pin_in=PIN_IN_5V;
//        break;
//      case TWELVE_VOLTS :
//        pin_in=PIN_IN_12V;
//        break;        
    }
    while(Serial.available()) Serial.read();
  }
}
