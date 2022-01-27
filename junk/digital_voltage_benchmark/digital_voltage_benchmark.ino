#include <TeensyTimerInterrupt.h>
#include <TeensyTimerInterrupt.hpp>

#define OUTPUT_PIN 5
#define INPUT_PIN 6
#define OUTPUT_PIN_ANALOG 14
IntervalTimer blink_timer;
IntervalTimer print_timer;
TeensyTimer ITimer0(TEENSY_TIMER_1);
int ledState = LOW;
int total = 0;

void blinkLED() {
  ledState = (ledState==LOW) ? HIGH : LOW;
  digitalWrite(LED_BUILTIN,ledState);
}

int counter = 0;

void print_total_voltage_reads() {
  counter++;
  if(counter == 40) {
    int this_total = total;
    total = 0;
    counter = 0;
    Serial.println("This second the teensy outputted and read this many voltages:");
    Serial.println(this_total);
  }
}

void setup() {
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN_ANALOG, INPUT);
  blink_timer.begin(blinkLED, 4000000);
//  print_timer.begin(print_total_voltage_reads, 1000000);
  ITimer0.attachInterruptInterval(50000, print_total_voltage_reads);
}

void loop() {
  digitalWrite(OUTPUT_PIN, LOW);
  if(digitalRead(INPUT_PIN)) {
    Serial.println("ERROR read false positive HIGH reading.");
  }
  total++;
  digitalWrite(OUTPUT_PIN, HIGH);
  if(!digitalRead(INPUT_PIN)) {
    Serial.println("ERROR failed to read HIGH correctly.");
  }
  total++;
}
