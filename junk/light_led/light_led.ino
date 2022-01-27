#define PIN_OUT 14
#define PIN_IN 15

void setup() {
  pinMode(PIN_IN,INPUT);
}

int brightness = 0;
bool upwards = true;

void loop() {
  analogWrite(PIN_OUT,brightness);
  if(upwards) {
    brightness++;
  } else {
    brightness--;
  }
  if(brightness == 20 || brightness == 0) upwards=!upwards;
  delay(100);
}
