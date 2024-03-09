#include "sd_card.h"
#include "log.h"
#include "rtc.h"
#include <SPI.h>
#include <SD.h>
#include <Arduino.h>

#define LOG_DIR "logs/"

void setup_log() {
  if(!file_exists(LOG_DIR)) {
    make_directory(LOG_DIR);
  }
}

bool log_file_open = false;
File current_log_file;
String date_dir;

void print_log_error() {
  Serial.println("log error.");
}

bool log_currently_open() {
  return log_file_open;
}

void start_new_log() {
  if(log_currently_open()) {
    close_log();
  }
  String timestamp = String(get_timestamp());
  date_dir = String("logs/") + timestamp.substring(0, 8);
  if(!file_exists_str(date_dir)) {
    make_directory_str(date_dir);
  }
  String time_str = timestamp.substring(9,17);
  String new_log_name = date_dir + "/" + time_str + ".txt";
  if(SD.exists(new_log_name)) {
    print_log_error();
    return;
  }
  current_log_file = SD.open(new_log_name, FILE_WRITE);
  log_file_open = true;
}

void print_current_log() {
  if(!log_currently_open()) {
    Serial.println("no log.");
    return;
  }
  Serial.println(current_log_file.name());
}

void save_log() {
  if(!log_currently_open()) {
    print_log_error();
    return;
  }
  current_log_file.flush();
}

void close_log() {
  if(!log_currently_open()) {
    print_log_error();
    return;
  }
  current_log_file.close();
  log_file_open = false;
}

void write_to_log(char* data, int num_chars) {
  if(!log_currently_open()) {
    print_log_error();
    return;
  }
  current_log_file.write(data, num_chars);
}

void write_to_log_bytes(byte* data, int num_bytes) {
  if(!log_currently_open()) {
    print_log_error();
    return;
  }
  current_log_file.write(data, num_bytes);
}

void run_log_benchmark(int num_bytes_per_flush, int num_flushes) {
  Serial.println("benchmarking writing to log...");
  if(log_currently_open()) {
    close_log();
  }
  long start = micros();
  start_new_log();
  long started_log = micros();
  for(int i = 0; i < num_flushes; i++) {
    byte* bytes_to_write = new byte[num_bytes_per_flush];
    for(int j = 0; j < num_bytes_per_flush; j++) {
      bytes_to_write[j] = j%256;
    }
    delete[] bytes_to_write;
  }
  long dry_run_complete = micros();
  for(int i = 0; i < num_flushes; i++) {
    byte* bytes_to_write = new byte[num_bytes_per_flush];
    for(int j = 0; j < num_bytes_per_flush; j++) {
      bytes_to_write[j] = j%256;
    }
    write_to_log_bytes(bytes_to_write, num_bytes_per_flush);
    save_log();
    delete[] bytes_to_write;
  }
  long writes_done = micros();

  long started_log_duration = started_log - start;
  long dry_run_duration = dry_run_complete - started_log;
  long writes_duration = writes_done - dry_run_complete;
  Serial.println("Wrote bytes to file: " + String(num_bytes_per_flush*num_flushes));
  Serial.println("Total relevant time taken: " + String(started_log_duration + writes_duration));
  Serial.println("started_log_duration: " + String(started_log_duration));
  Serial.println("dry_run_duration: " + String(dry_run_duration));
  Serial.println("writes_duration: " + String(writes_duration));
  Serial.println("cleaning up...");
  close_log();
  String log_name = current_log_file.name();
  String file_name = date_dir + +"/" + log_name;
  Serial.println("Deleting: " + file_name);
  rm_file_str(file_name);

  //this was found to take 77ms to start a log,
  // 137 ms to perform the dry run (i.e. allocate, set, and deallocate 512kb of memory)
  // 121 seconds to perform the writing to the log
  // 
}
