#define INPUT_POWER_AMPLITUDE_PIN 16
#define INPUT_AMPLITUDE_PIN 18

#define INPUT_POWER_FREQUENCY_PIN 20
#define INPUT_FREQUENCY_PIN 22

void setup_input() {
  pinMode(INPUT_POWER_FREQUENCY_PIN, OUTPUT);
  pinMode(INPUT_POWER_AMPLITUDE_PIN, OUTPUT);
  digitalWrite(INPUT_POWER_FREQUENCY_PIN, LOW);
  digitalWrite(INPUT_POWER_FREQUENCY_PIN, LOW);
  pinMode(INPUT_FREQUENCY_PIN, INPUT_DISABLE);
  pinMode(INPUT_AMPLITUDE_PIN, INPUT_DISABLE);
}

uint16_t read_input(uint16_t pin, uint16_t power_pin) {
  digitalWrite(power_pin, HIGH);
  long results = 0;
  for(int i = 0; i<16;i++) {
    results += analogRead(pin);
  }
  uint16_t res = results/16;
  digitalWrite(power_pin, LOW);
  Serial.print("measured input as: ");
  Serial.println(res);
  return res;
}

long convert_frequency_input_to_frequency(uint16_t frequency_input) {
  long freq = frequency_input;
  return freq*10000;
}

void handle_frequency_input() {
  uint16_t reading = read_input(INPUT_FREQUENCY_PIN, INPUT_POWER_FREQUENCY_PIN);
  long frequency = convert_frequency_input_to_frequency(reading);
  Serial.print("setting frequency: ");
  Serial.println(frequency);
  wait_micros = reading;
}

void handle_amplitude_input() {
  uint16_t reading = read_input(INPUT_AMPLITUDE_PIN, INPUT_POWER_AMPLITUDE_PIN);
  Serial.print("setting current y scale: ");
  Serial.println(reading);
  current_y_scale = reading;
}

void handle_input() {
  handle_frequency_input();
  handle_amplitude_input();
}
