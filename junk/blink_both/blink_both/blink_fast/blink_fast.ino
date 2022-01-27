// Simple LED blink

const int led = LED_BUILTIN;

void setup() {
  pinMode(led, OUTPUT);
}

// 100 == full brightness, 0 == nothing
// takes 100 micro seconds
void light(int brightness) {
      digitalWrite(led, HIGH);
      delayMicroseconds(brightness);
      digitalWrite(led, LOW);
      delayMicroseconds(100-brightness);
}

int brightness = 0;
bool forwards = true;

void loop() {
  Serial.println();
  for(int i = 0; i < 100; i++) light(brightness);
  if(forwards) {
    brightness++;
    if(brightness==100) forwards = false;
  } else {
    brightness--;
    if(brightness==0) forwards = true;
  }
}
