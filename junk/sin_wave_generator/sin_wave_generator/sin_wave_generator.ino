/*
 * This sketch reads from a variety of input pins and writes the results to a file on the sd card, in csv format.
 * The file it writes to is named after the current epoch time when the program starts. 
 * The sketch might also read input from serial in, to perform such things as:
 *  - updating the sampling frequency
 *  - updating the writing frequency (if we read multiple values before writing them all at once?
 *  - starting a new file
 *  - stopping the data collection and instead doing something else like sending a ls of files or sending a file through serial.
 */
#include "input.h"
#include "sd_card.h"
#include "log.h"

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
  setup_running_indicator();
  setup_rtc();
  setup_sd_card();
  setup_log();
  delay(100);
}

void loop() {
  handle_running_indicator();
  handle_input();
  delay(100);
}
