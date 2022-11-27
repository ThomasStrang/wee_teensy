//input a is gp28, pin 34
#define INPUT_A 28
//input b is gp26, pin 31
#define INPUT_B 26

//For reference, other pins connected are:
//pin 39 -- 5V input
//pin 38 -- ground output, 68k resistor going to pin 34 
//pin 34 -- reading voltage, 68k resistor going to pin 38, 200k resistor going to signal input for speedometer
//pin 30 -- reading voltage, 68k resistor going to pin 28, 200k resistor going to signal input for tachometer
//pin 28 -- ground, 68k resistor going to pin 31

void setup_inputs() {
  pinMode(INPUT_A,INPUT);
  pinMode(INPUT_B,INPUT);
}

void handle_left() {
  Serial.println("a");
}

void handle_right() {
  Serial.println("d");
}

void handle_up() {
  Serial.println("w");
}

void handle_down() {
  Serial.println("s");
}

void handle_forward_slash() {
  Serial.println("/");
}

void handle_p() {
  Serial.println("p");
}

void handle_input_a(int i) {
  update_input_voltage(i);
}

void handle_input_b(int i) {
  //DO NOTHING.
}

void handle_input() {
  handle_input_a(analogRead(INPUT_A));
  handle_input_b(analogRead(INPUT_B));
  while(Serial.available()) {
    switch(Serial.read()) {
      case 'A':
      handle_left();
      break;
      case 'S':
      handle_down();
      break;
      case 'D':
      handle_right();
      break;
      case 'W':
      handle_up();
      break;
      case '/':
      handle_forward_slash();
      case 'P':
      handle_p();
      break;
    }
  }
  while(Serial.available()) Serial.read();//consume any unused input.
}
