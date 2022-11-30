#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "display.h"
#include "frequency_measurement.h"

void setup() {
  analogWrite(21,127);
  setup_display();
}

void loop() {
  handle_input();
  delay(100);
}
