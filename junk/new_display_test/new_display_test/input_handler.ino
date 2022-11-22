void handle_left() {
  Serial.println("left");
  graph.move_graph(graph.x_offset-5, graph.y_offset);
}

void handle_right() {
  Serial.println("right");
  graph.move_graph(graph.x_offset+5, graph.y_offset);
}

void handle_up() {
  Serial.println("up");
  graph.move_graph(graph.x_offset, graph.y_offset-5);
}

void handle_down() {
  Serial.println("down");
  graph.move_graph(graph.x_offset, graph.y_offset+5);
}

void handle_forward_slash() {
  for(float i =0 ; i < 100; i+=0.5) {
    Serial.print(i);
    Serial.print(" -> ");
    Serial.println(i*100);
    graph.draw(i,i*100);
  }
}

void handle_p() {
  draw_pikachu();
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
