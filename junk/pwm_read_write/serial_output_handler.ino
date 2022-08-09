void send_to_serial_plotter(int* vals, int num) {
  for(int i = 0; i < num; i++) {
    if(analogue) {
      Serial.println(convert_to_volts(vals[i]));
    } else {
      Serial.println(vals[i]);
    }
  }
}

void send_to_serial_monitor(String val, float f) {
  Serial.print(val);
  Serial.println(f);
}


void send_to_serial_monitor(float f, String s) {
  Serial.print(f);
  Serial.println(s);
}

void send_to_serial_monitor(float f) {
  Serial.println(f);
}
void send_to_serial_monitor(String s) {
  Serial.println(s);
}
