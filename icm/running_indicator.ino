#define MILLIS_PER_RADIAN 500.0
void setup_running_indicator() {
  pinMode(LED_BUILTIN, OUTPUT);
}

int get_led_level(long time) {
    float rads = float(time) / MILLIS_PER_RADIAN;
    double sin_val = sin(rads);
    return int((sin_val + 1.0) * 128);
}

void handle_running_indicator() {
  long now = millis();
  analogWrite(LED_BUILTIN, get_led_level(now));
}
