
void setup() {
  if(setup_display()) {
    Serial.println("ERROR, setup_display() != 0");
  }
}

void loop() {
  //this function call takes 200us
  //the brightness of the leds scales with with how often this function is called.
  //It is best to run this in it's own thread.
  light_digits(1,2,3,4);
  delay(1);
}
