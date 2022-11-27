
//inputs are connected to ground via 68000 ohms and to the terminals via 200000 ohms. 
//total resistance 268000 ohms. 
//voltage reduction is 68/268 -> 13V is read as 3.3V. 
//1023 means 13V, so reading to 
// Not sure on the specifics as to why, but the pico gives a reading of  22 even when connected to ground.
#define READING_TO_VOLTAGE 0.01270772238

char new_voltage_str[] = "00.00V";
void update_drawn_voltage(float f) {
  new_voltage_str[0]=int(f/10)+48;//48 because of the ascii table
  new_voltage_str[1]=int(f)%10+48;
  new_voltage_str[2]='.';
  new_voltage_str[3]=int(f*10)%10+48;
  new_voltage_str[4]=int(f*100)%10+48;
  new_voltage_str[5]='V';
  voltage_drawer.update_string(new_voltage_str);
}

void update_input_voltage(int input_reading) {
  update_drawn_voltage(input_reading*READING_TO_VOLTAGE);
  voltage_graph_drawer.add_val_to_end(input_reading);
}
