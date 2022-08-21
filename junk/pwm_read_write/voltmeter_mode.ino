/*
 * in Voltmeter mode the voltage is displayed (serial monitor)
 */

void generate_voltmeter_output(){
  pinMode(pin_in,INPUT_DISABLE);
  float voltage = convert_to_volts(analogRead(pin_in));
  pinMode(pin_in,analogue ? INPUT_DISABLE : INPUT);
  send_to_serial_monitor(voltage);
}

float voltmeter_time_to_wait(){
  return screen_delay_ms;
}
