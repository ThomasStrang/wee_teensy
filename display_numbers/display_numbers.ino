
#define INPUT_PIN 22
#define OUTPUT_PIN 21

void setup() {
  if(setup_display()) {
    Serial.println("ERROR, setup_display() != 0");
  }
  pinMode(INPUT_PIN,INPUT);
  pinMode(OUTPUT_PIN,OUTPUT);
  digitalWrite(OUTPUT_PIN,HIGH);
}

void print_number(int n) {
  int ones = n%10;
  int tens = (n/10)%10;
  int hundreds = (n/100)%10;
  int thousands = (n/1000)%10;
  light_digits(thousands,hundreds,tens,ones);
}

void loop() {
  int input_value = analogRead(INPUT_PIN);
  int start = millis();
  while(millis()-start < 500) {
    print_number(input_value);
    delay(1);
  }
}
