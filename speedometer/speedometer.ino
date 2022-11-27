void setup() {
  analogWrite(21,127);
  setup_display();
}

void loop() {
  handle_input();
  delay(100);
}
