
#define INPUT_PIN 22
#define GROUND_PIN 5
#define VOLTS_PIN 6
#define OUTPUT_PIN 23
IntervalTimer switch_voltage_timer;
IntervalTimer input_value_timer;

byte setup_voltages() {
  pinMode(INPUT_PIN,INPUT);
  
  pinMode(GROUND_PIN,OUTPUT);
  digitalWrite(GROUND_PIN,LOW);
  pinMode(VOLTS_PIN,OUTPUT);
  digitalWrite(VOLTS_PIN,HIGH);
  pinMode(OUTPUT_PIN,OUTPUT);
  switch_voltage_timer.begin(switch_voltage, 2000000);
  input_value_timer.begin(refresh_input_value, 250000);
  return 0;
}

void setup() {
  if(setup_display()) Serial.println("ERROR, setup_display() != 0");
  if(setup_voltages()) Serial.println("ERROR, setup_voltages() != 0");
}

int voltage=HIGH;

void switch_voltage() {
  voltage = (voltage==LOW) ? HIGH : LOW;
  digitalWrite(OUTPUT_PIN,voltage);
  digitalWrite(LED_BUILTIN,voltage);
}

void print_number(int n) {
  int ones = n%10;
  int tens = (n/10)%10;
  int hundreds = (n/100)%10;
  int thousands = (n/1000)%10;
  light_digits(thousands,hundreds,tens,ones);
}


float value_granularity = 1.024;//number of bits returned by sensor
float max_voltage = 3.3;
int display_value=0;

void refresh_input_value() {
  int value = analogRead(INPUT_PIN);
  display_value = (value/value_granularity) * max_voltage;
}

void loop() {
  print_number(display_value);
}
