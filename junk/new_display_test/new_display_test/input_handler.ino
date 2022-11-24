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
  draw_pikachu();
}

void handle_input() {
  pinMode(18,INPUT);
  update_input_voltage(digitalRead(18));
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
