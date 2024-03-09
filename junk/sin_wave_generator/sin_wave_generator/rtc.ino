#include <STM32RTC.h>
#include "rtc.h"

STM32RTC& rtc = STM32RTC::getInstance();

void setup_rtc() {
//  rtc.setClockSource(STM32RTC::HSE_CLOCK);
  rtc.begin(); // initialize RTC 24H format
}


char rtc_buf[20];

void fill_timestamp() {
  time_t rawtime = rtc.getEpoch();
  struct tm ts;
  ts = *localtime(&rawtime);
  strftime(rtc_buf, 18, "%y-%m-%d_%H:%M:%S", &ts);
}

uint32_t get_epoch() {
  return rtc.getEpoch();
}

void print_rtc() {
  fill_timestamp();
  Serial.print("Unix time = ");
  Serial.print(get_epoch());
  Serial.print(" - ");
  Serial.println(rtc_buf);
}

void set_clock(uint32_t seconds) {
  rtc.setEpoch(seconds);
}

char* get_timestamp() {
  fill_timestamp();
  return rtc_buf;
}
