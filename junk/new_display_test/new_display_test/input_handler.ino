//input a is gp28, pin 34
#define INPUT_A 28
//input b is gp26, ping 31
#define INPUT_B 26


void handle_left() {
  Serial.println("left");
}

void handle_right() {
  Serial.println("right");
}

void handle_up() {
  Serial.println("up");
}

void handle_down() {
  Serial.println("down");
}

void handle_forward_slash() {
  for(float i =0 ; i < 100; i+=0.5) {
    Serial.print(i);
  }
}

void handle_p() {
  Serial.println("p?");
}

void handle_input() {
  pinMode(INPUT_A,INPUT);
  update_input_voltage(analogRead(INPUT_A));
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
      case 'p':
      case 'P':
      handle_p();
      break;
    }
  }
  while(Serial.available()) Serial.read();
}
