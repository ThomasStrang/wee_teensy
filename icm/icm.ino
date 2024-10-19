
void setup() {
  setup_input_signal();
  setup_running_indicator();
}

void loop() {
  handle_running_indicator();
  handle_input_signal();
}
