/*
 * This sketch is used to generate an ac sin wave from the DAC
 * A voltage divider (2 resistors) can be used to provide a 'ground' which is half the reference voltage of this board.
 * Then an ac sin wave can be measured between these 2 points using an oscilloscope.
 * 
 */
#include <SPI.h>
#include <SD.h>

void setup() {
  setup_sin_wave();
  setup_running_indicator();
  Serial.begin(9600);
  while(!Serial){}
  delay(3000);
  run_example_sd_card_program();
}

void loop() {
  handle_running_indicator();
  handle_sin_wave();
  Serial.println("THis ran.");
}
