#include <STM32RTC.h>
#include "rtc.h"

STM32RTC& rtc = STM32RTC::getInstance();

void setup_rtc() {
//  rtc.setClockSource(STM32RTC::HSE_CLOCK);
  rtc.begin(); // initialize RTC 24H format
}

void get_timestamp(char* buf) {
  time_t rawtime = rtc.getEpoch();
  struct tm ts;
  ts = *localtime(&rawtime);
  strftime(buf, 80, "%a %Y-%m-%d %H:%M:%S", &ts);
}

uint32_t get_epoch() {
  return rtc.getEpoch();
}

void print_rtc() {
  char buf[80];
  get_timestamp(buf);
 
  Serial.print("Unix time = ");
  Serial.print(get_epoch());
  Serial.print(" - ");
  Serial.println(buf);
}

void set_clock(uint32_t seconds) {
  rtc.setEpoch(seconds);
}
