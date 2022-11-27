/*
 * In oscilloscope mode an oscilloscope chart is displayed. (serial plotter)
 */

void generate_oscilloscope_graph() {
  int vals[500];
  for(int i = 0; i < 500; i++) {
    vals[i]=get_val();
    delayMicroseconds(read_delay_micros);
  }
  send_to_serial_plotter(vals,500);
}

float oscilloscope_time_to_wait(){
  int time_to_read = read_delay_micros>>1;//using micros in a millisecond context multiplies the time by 1000. bitshifting by 1 divides by 2, resulting in total multiplication of 500.
  int time_to_wait = screen_delay_ms - time_to_read;//technically this doesn't take into account the time to display (or handling input), but it's only ~0.1 milliseconds so it doesn't really matter.
  return time_to_wait>=0?time_to_wait:0;
}
