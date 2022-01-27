#define PIN_OUT 14
#define PIN_IN 15

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_IN, INPUT);
  calibrate_normalise_val();
}

float normalise_val = 0.333;

void calibrate_normalise_val() {
  normalise_val=1.0/3.0;
  analogWrite(PIN_OUT,255);
  float raw = read_val();
  normalise_val = (255.0/raw)/3;
}

int read_val() {
  int first = pulseIn(PIN_IN, HIGH);
  int second = pulseIn(PIN_IN, HIGH);
  int third = pulseIn(PIN_IN, HIGH);
  
  return (int)((first+second+third)*normalise_val);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 256; i++) {
    analogWrite(PIN_OUT,i);
    int val = read_val();
    Serial.print("\nvalue sent: ");
    Serial.print(i);
    Serial.print(" value read: ");
    Serial.print(val);
    delay(100);
  }
}
