void setup_running_indicator() {
  pinMode(LED_YELLOW,OUTPUT);
}

int seconds_passed(){
  return (int) (millis()/1000);
}

bool running_indicator_led_should_be_high() {
  return seconds_passed()%2;
}

void handle_running_indicator() {
  digitalWrite(LED_YELLOW, running_indicator_led_should_be_high());
}
