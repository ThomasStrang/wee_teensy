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
  unsigned long min_iteration_duration;
  unsigned long max_iteration_duration;
  unsigned long last_iteration_duration;
  
  unsigned long last_iteration_start;
  unsigned long iteration_end;
  for(int i = 0; i < num_flushes; i++) {
    byte* bytes_to_write = new byte[num_bytes_per_flush];
    for(int j = 0; j < num_bytes_per_flush; j++) {
      bytes_to_write[j] = j%256;
    }
    last_iteration_start = micros();
    write_to_log_bytes(bytes_to_write, num_bytes_per_flush);
    iteration_end = micros();
    last_iteration_duration = iteration_end - last_iteration_start;
    if(min_iteration_duration==0 || last_iteration_duration < min_iteration_duration) {
      min_iteration_duration=last_iteration_duration;
    }
    if(max_iteration_duration < last_iteration_duration) {
      max_iteration_duration = last_iteration_duration;
    }
//    save_log();
    delete[] bytes_to_write;
  }
  save_log();
  long writes_done = micros();

  long started_log_duration = started_log - start;
  long dry_run_duration = dry_run_complete - started_log;
  long writes_duration = writes_done - dry_run_complete;
  Serial.println("Wrote bytes to file: " + String(num_bytes_per_flush*num_flushes));
  Serial.println("Total relevant time taken: " + String(started_log_duration + writes_duration));
  Serial.println("started_log_duration: " + String(started_log_duration));
  Serial.println("dry_run_duration: " + String(dry_run_duration));
  Serial.println("writes_duration: " + String(writes_duration));
  Serial.print("max_iteration_duration: ");
  Serial.println(max_iteration_duration);
  Serial.println("min_iteration_duration: " + String(min_iteration_duration));
  Serial.println("last_iteration_duration: " + String(last_iteration_duration));
  Serial.println("cleaning up...");
  close_log();
  String log_name = current_log_file.name();
  String file_name = date_dir + +"/" + log_name;
  Serial.println("Deleting: " + file_name);
  rm_file_str(file_name);

  /* this was found to take 77ms to start a log,
   * 137 ms to perform the dry run (i.e. allocate, set, and deallocate 512kb of memory)
   * 121 seconds to perform the writing to the log
   * that is equivalent to 15ms per log flush of 64 bytes.
   * at 12000rpm with 36 teeth on the eccentric shaft, that's 7200 teeth per second. 
   * so flushing to the log file would take 108 teeth?
   * 
   * without constant flushing it took 4.5 seconds. not sure how often it flushed...
   *    Wrote bytes to file: 524288
        Total relevant time taken: 4871743
        started_log_duration: 77550
        dry_run_duration: 145352
        writes_duration: 4794193
   * 
   * And with some extra logging:
   *    Wrote bytes to file: 524288
        Total relevant time taken: 4917831
        started_log_duration: 77550
        dry_run_duration: 145689
        writes_duration: 4840281
        max_iteration_duration: 32591
        32591
        min_iteration_duration: 19
        last_iteration_duration: 20
        cleaning up...
   * 
   * looks like it takes about 30ms to flush the whole SD buffer, or 1/30th of a second. that's terribly slow...
   * we can use unsigned int chunkSize = myFile.availableForWrite(); to never write to the sd card when the buffer is full.
   */
}
