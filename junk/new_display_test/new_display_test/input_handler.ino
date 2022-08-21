void handle_left() {
  Serial.println("left");
  square.p.x--;
}

void handle_right() {
  Serial.println("right");
  square.p.x++;
}

void handle_up() {
  Serial.println("up");
  square.p.y--;
  
}

void handle_down() {
  Serial.println("down");
  square.p.y++;
  
}

void handle_input() {
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
      case 'V':
      square.viewable=!square.viewable;
      break;
    }
  }
  while(Serial.available()) Serial.read();
}
