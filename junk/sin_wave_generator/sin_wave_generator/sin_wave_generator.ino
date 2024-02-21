/*
 * This sketch is used to generate an ac sin wave from the DAC
 * A voltage divider (2 resistors) can be used to provide a 'ground' which is half the reference voltage of this board.
 * Then an ac sin wave can be measured between these 2 points using an oscilloscope.
 * 
 */

void setup() {
  Serial.begin(9600);
  while(!Serial){}
  setup_sin_wave();
  setup_running_indicator();
  setup_sd_card();
  delay(1500);
  setup_storage_file();
}

void loop() {
  handle_running_indicator();
  handle_sin_wave();
  handle_writing_to_file();
}
