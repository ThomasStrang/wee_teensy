//NAME        TEENSY PORT      COLOUR      DISPLAY PORT
//CHAR_1      1                WHITE       1
//CHAR_2      2                GREY        2
//CHAR_3      3                PURPLE      6
//CHAR_4      4                BLUE        8


//TOP         14               BROWN       14
//TOP_LEFT    15               RED         11
//TOP_RIGHT   16               BLACK       16
//MIDDLE      17               GREEN       15
//BOT_LEFT    18               YELLOW      5
//BOT_RIGHT   19               ORANGE      13
//BOT         20               RED         3
//DOT         21               ORANGE      7

#define TOP 14
#define TOP_LEFT 15
#define TOP_RIGHT 16
#define MIDDLE 17
#define BOT_LEFT 18
#define BOT_RIGHT 19
#define BOT 20
#define DOT 21

//brightness is PWM thickness, so between 0 and 255 at 4482hz. however 255 would definitely blow the leds, 20 is bright. 
#define BRIGHTNESS 5

void all_digits_off() {
  analogWrite(0, 0); 
  analogWrite(1, 0); 
  analogWrite(2, 0); 
  analogWrite(3, 0); 
}

void all_digits_on() {
  analogWrite(0, BRIGHTNESS); 
  analogWrite(1, BRIGHTNESS); 
  analogWrite(2, BRIGHTNESS); 
  analogWrite(3, BRIGHTNESS);
}

void all_values_off() {
   digitalWrite(TOP,HIGH);
   digitalWrite(TOP_LEFT,HIGH);
   digitalWrite(TOP_RIGHT,HIGH);
   digitalWrite(MIDDLE,HIGH);
   digitalWrite(BOT_LEFT,HIGH);
   digitalWrite(BOT_RIGHT,HIGH);
   digitalWrite(BOT,HIGH);
   digitalWrite(DOT, HIGH);
}

void all_off() {
  all_digits_off();
  all_values_off();
}

void light_number(int value) {
  switch(value) {
    case 0:
     digitalWrite(TOP,LOW);
     digitalWrite(TOP_LEFT,LOW);
     digitalWrite(TOP_RIGHT,LOW);
     digitalWrite(MIDDLE,HIGH);
     digitalWrite(BOT_LEFT,LOW);
     digitalWrite(BOT_RIGHT,LOW);
     digitalWrite(BOT,LOW);
     break;
    case 1:
     digitalWrite(TOP,HIGH);
     digitalWrite(TOP_LEFT,HIGH);
     digitalWrite(TOP_RIGHT,LOW);
     digitalWrite(MIDDLE,HIGH);
     digitalWrite(BOT_LEFT,HIGH);
     digitalWrite(BOT_RIGHT,LOW);
     digitalWrite(BOT,HIGH);
     break;
    case 2:
     digitalWrite(TOP,LOW);
     digitalWrite(TOP_LEFT,HIGH);
     digitalWrite(TOP_RIGHT,LOW);
     digitalWrite(MIDDLE,LOW);
     digitalWrite(BOT_LEFT,LOW);
     digitalWrite(BOT_RIGHT,HIGH);
     digitalWrite(BOT,LOW);
     break;
    case 3:
     digitalWrite(TOP,LOW);
     digitalWrite(TOP_LEFT,HIGH);
     digitalWrite(TOP_RIGHT,LOW);
     digitalWrite(MIDDLE,LOW);
     digitalWrite(BOT_LEFT,HIGH);
     digitalWrite(BOT_RIGHT,LOW);
     digitalWrite(BOT,LOW);
     break;
    case 4:
     digitalWrite(TOP,HIGH);
     digitalWrite(TOP_LEFT,LOW);
     digitalWrite(TOP_RIGHT,LOW);
     digitalWrite(MIDDLE,LOW);
     digitalWrite(BOT_LEFT,HIGH);
     digitalWrite(BOT_RIGHT,LOW);
     digitalWrite(BOT,HIGH);
     break;
    case 5:
     digitalWrite(TOP,LOW);
     digitalWrite(TOP_LEFT,LOW);
     digitalWrite(TOP_RIGHT,HIGH);
     digitalWrite(MIDDLE,LOW);
     digitalWrite(BOT_LEFT,HIGH);
     digitalWrite(BOT_RIGHT,LOW);
     digitalWrite(BOT,LOW);
     break;
    case 6:
     digitalWrite(TOP,LOW);
     digitalWrite(TOP_LEFT,LOW);
     digitalWrite(TOP_RIGHT,HIGH);
     digitalWrite(MIDDLE,LOW);
     digitalWrite(BOT_LEFT,LOW);
     digitalWrite(BOT_RIGHT,LOW);
     digitalWrite(BOT,LOW);
     break;
    case 7:
     digitalWrite(TOP,LOW);
     digitalWrite(TOP_LEFT,HIGH);
     digitalWrite(TOP_RIGHT,LOW);
     digitalWrite(MIDDLE,HIGH);
     digitalWrite(BOT_LEFT,HIGH);
     digitalWrite(BOT_RIGHT,LOW);
     digitalWrite(BOT,HIGH);
     break;
    case 8:
     digitalWrite(TOP,LOW);
     digitalWrite(TOP_LEFT,LOW);
     digitalWrite(TOP_RIGHT,LOW);
     digitalWrite(MIDDLE,LOW);
     digitalWrite(BOT_LEFT,LOW);
     digitalWrite(BOT_RIGHT,LOW);
     digitalWrite(BOT,LOW);
     break;
    case 9:
     digitalWrite(TOP,LOW);
     digitalWrite(TOP_LEFT,LOW);
     digitalWrite(TOP_RIGHT,LOW);
     digitalWrite(MIDDLE,LOW);
     digitalWrite(BOT_LEFT,HIGH);
     digitalWrite(BOT_RIGHT,LOW);
     digitalWrite(BOT,LOW);
     break;
  }
}

void light_digit(int digit, int value, int brightness) {
  all_digits_off();
  all_values_off();
  analogWrite(digit, brightness);
  light_number(value);
}

//function takes 200 micro seconds
void light_digits(int digit_1, int digit_2, int digit_3, int digit_4, int brightness) {
  light_digit(0,digit_1,brightness);
  digitalWrite(DOT,LOW);//add the dot
  delayMicroseconds(50);
  light_digit(1,digit_2,brightness);
  delayMicroseconds(50);
  light_digit(2,digit_3,brightness);
  delayMicroseconds(50);
  light_digit(3,digit_4,brightness);
  delayMicroseconds(50);
  all_off();
}

void light_digits(int digit_1, int digit_2, int digit_3, int digit_4) {
  light_digits(digit_1, digit_2, digit_3, digit_4, 10);
}

byte setup_display() {
   pinMode(TOP, OUTPUT);
   pinMode(TOP_LEFT, OUTPUT);
   pinMode(TOP_RIGHT, OUTPUT);
   pinMode(MIDDLE, OUTPUT);
   pinMode(BOT_LEFT, OUTPUT);
   pinMode(BOT_RIGHT, OUTPUT);
   pinMode(BOT, OUTPUT);
   pinMode(DOT, OUTPUT);
   analogWriteFrequency(0,585000);
   analogWriteFrequency(1,585000);
   analogWriteFrequency(2,585000);
   analogWriteFrequency(3,585000);
   all_off();
   return 0;
}
