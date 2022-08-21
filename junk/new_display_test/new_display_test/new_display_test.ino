#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "sprite_drawer.cpp"

Adafruit_ST7735 tft = Adafruit_ST7735(&SPI1,9,8,12);
Square square = Square(&tft);
Position v=Position(3,5);

char text[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserun";

uint16_t parse_colour(uint16_t r, uint16_t g, uint16_t b) {
  return (g<<10)+(r<<5)+(b);
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void move_square() {
  if(square.p.x < 0) {
    Serial.println("bouncing off left");
    v.x=3;
  } else if(square.p.x > 150) {
    Serial.println("bouncing off right");
    v.x=-3;
  }
  if(square.p.y < 0) {
    Serial.println("bouncing off top");
    v.y=5;
  } else if(square.p.y > 118) {
    Serial.println("bouncing off bottom");
    v.y=-5;
  }
  square.p.x=square.p.x+v.x;
  square.p.y=square.p.y+v.y;
}

long reset_counter=0;

void reset_program() {
  tft.fillScreen(0x0000);
  testdrawtext(text, parse_colour(32,32,32));
  square.viewable=true;
  square._undraw();
  reset_counter=millis()+65000;
}

void setup() {
  SPI1.setCS(9);
  SPI1.setSCK(10);
  SPI1.setTX(11);
  SPI1.begin(true);
  tft.initR(INITR_GREENTAB);
  tft.setRotation(3);
  pinMode(13, OUTPUT);
  analogWrite(13, 32);
  reset_program();
}

void loop() {
  if(millis()>reset_counter) reset_program();
  handle_input();
  move_square();
  square.draw();
  delay(100);
}
