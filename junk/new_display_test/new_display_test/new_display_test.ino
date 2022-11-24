void setup() {
  analogWrite(21,127);
  setup_rendering();
}

void loop() {
  handle_input();
  delay(100);
  draw_pikachu();
}
