#include <Adafruit_SharpMem.h>

#define NUM_VALS_IN_GRAPH 380
#define GRAPH_Y_SCALE 260
#define NUM_Y_PIXELS_IN_GRAPH 200

#define BLACK 0
#define WHITE 1

#define MAX_VOLTAGE 15

uint16_t wait_micros = 0;//micros to wait between making readings
float current_reading_frequency = 1.0; //calculated frequency of last reading
uint16_t current_y_scale = 255; //the number which is the 'top' of the graph. 

void setup() {
  setup_input();
  setup_signal();
  setup_voltage_reading();
  setup_display();
}

void loop() {
  handle_input();
  update_signal();
  sync_with_signal();
  uint16_t* voltages = read_voltages();
  draw(voltages);
  delay(100);
}
