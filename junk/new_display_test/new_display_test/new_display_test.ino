void setup() {
  analogWrite(21,127);
  setup_rendering();
}

void loop() {
  handle_input();
  draw_stuff();
  delay(100);
}
