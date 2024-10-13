#include <Arduino.h>
#include "sd_card.h"
#include "SdFat.h"

#define SD_FAT_TYPE 2

const uint32_t LOG_INTERVAL_USEC = 2000;
#define USE_RTC 0

#define ERROR_LED_PIN LED_GREEN
const uint8_t SD_CS_PIN = 25;

#define FIFO_SIZE_SECTORS 16

const uint32_t PREALLOCATE_SIZE_MiB = 10UL;
#define SPI_CLOCK SD_SCK_MHZ(50)

#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)

const size_t ADC_COUNT = 4;
struct DataType {
  uint16_t adcs[ADC_COUNT];
};

void logRecord(data_t* data, uint16_t overrun) {
  if (overrun) {
    // Add one since this record has no adc data. Could add overrun field.
    overrun++;
    data->adc[0] = 0X8000 | overrun;
  } else {
    for (size_t i = 0; i < ADC_COUNT; i++) {
      data->adc[i] = analogRead(i);
    }
  }
}




#include <SPI.h>
#include <SD.h>

void setup_sd_card() {
  SPI.setMISO(33);
  SPI.setMOSI(34);
  SPI.setSCLK(32);
  SD.begin(25);
  SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE3));
}


void print_directory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      print_directory(entry, numTabs + 1);
    } else {
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void print_file_tree() {
  File root = SD.open("/");
  print_directory(root, 0);
  root.close();
}

void print_invalid_filename() {
  Serial.println("invalid filename");
}

void print_failure() {
  Serial.println("error.");
}

void make_file(char* name_of_file) {
  String filename = String(name_of_file);
  for (int i = filename.length() - 1; i >= 0; i--) {
    if (filename.charAt(i) == '/') {
      name_of_file[i] = '\0';
      bool directory_exists = SD.exists(String(name_of_file));
      name_of_file[i] = '/';
      if (!directory_exists) {
        print_invalid_filename();
        return;
      } else {
        break;
      }
    }
  }
  if (SD.exists(filename)) {
    print_invalid_filename();
    return;
  }
  File f = SD.open(filename, O_WRITE | O_CREAT);
  if (f) {
    f.close();
  } else {
    print_failure();
  }
}

void make_directory_str(String filename) {
  if (SD.exists(filename)) {
    print_invalid_filename();
    return;
  }
  if (!SD.mkdir(filename)) {
    print_failure();
  }
}

void make_directory(char* name_of_directory) {
  String filename = String(name_of_directory);
  make_directory_str(filename);
}

bool file_exists_str(String filename) {
  return SD.exists(filename);
}

bool file_exists(char* name_of_file) {
  String filename = String(name_of_file);
  return file_exists_str(filename);
}

void rm_file_str(String filename) {
  if (!SD.exists(filename)) {
    print_invalid_filename();
    return;
  }
  if (!SD.remove(filename)) {
    print_failure();
  }
}

void rm_file(char* name_of_file) {
  String filename = String(name_of_file);
  rm_file_str(filename);
}

void rm_directory(char* name_of_directory) {
  String filename = String(name_of_directory);
  if (!SD.exists(filename)) {
    print_invalid_filename();
    return;
  }
  if (!SD.rmdir(filename)) {
    print_failure();
  }
}

void print_file_contents(char* name_of_file) {
  String filename = String(name_of_file);
  if (!SD.exists(filename)) {
    print_invalid_filename();
    return;
  }
  File f = SD.open(filename);
  if (f) {
    while (f.available()) {
      Serial.write(f.read());
    }
    f.close();
    Serial.println();
  } else {
    print_failure();
  }
}

File curr_file;



//log stuff

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
  current_log_file = SD.open(new_log_name, O_WRITE | O_CREAT);
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

void run_log_benchmark() {
  int num_bytes_per_flush = 512;
  int num_flushes = 1024;
  int preallocated_file_size = num_bytes_per_flush * num_flushes;
  byte* bytes_to_write = new byte[num_bytes_per_flush];
  Serial.println("benchmarking writing to log...");
  if(log_currently_open()) {
    close_log();
  }
  long start = micros();
  start_new_log();
  Serial.println("Starting benchmark. File size is: " + String(current_log_file.size()));
  Serial.println("Initial available for write: " + String(current_log_file.availableForWrite()));
  long started_log = micros();
  unsigned long min_iteration_duration=0;
  unsigned long max_iteration_duration=0;
  unsigned long last_iteration_duration=0;
  
  unsigned long last_iteration_start;
  unsigned long iteration_end;
  for(int j = 0; j < num_bytes_per_flush; j++) {
    bytes_to_write[j] = j%256;
  }
  unsigned int max_available = current_log_file.availableForWrite();
  int num_iterations_blocked = 0;
  for(int i = 0; i < num_flushes; i++) {
    unsigned int available_to_write = current_log_file.availableForWrite();
    
    last_iteration_start = micros();
    write_to_log_bytes(bytes_to_write, num_bytes_per_flush);
    iteration_end = micros();
    last_iteration_duration = iteration_end - last_iteration_start;
//    Serial.println(last_iteration_duration);
    if(min_iteration_duration==0 || last_iteration_duration < min_iteration_duration) {
      min_iteration_duration=last_iteration_duration;
    }
    if(max_iteration_duration < last_iteration_duration) {
      max_iteration_duration = last_iteration_duration;
    }
  }
  delete[] bytes_to_write;
  save_log();
  long writes_done = micros();

  long started_log_duration = started_log - start;
  long writes_duration = writes_done - started_log;
  Serial.println("Wrote bytes to file: " + String(num_bytes_per_flush*num_flushes));
  Serial.println("Total relevant time taken: " + String(started_log_duration + writes_duration));
  Serial.println("started_log_duration: " + String(started_log_duration));
  Serial.println("writes_duration: " + String(writes_duration));
  Serial.println("Max available in buffer: " + String(max_available));
  Serial.println("number iterations: " + String(num_flushes));
  Serial.println("number of iterations blocked: " + String(num_iterations_blocked));
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
   * 
   * 11.03.2024 - going to run some more testing but this time with 512 bytes per write to try to make it 1 flush per write.
   * 512 bytes, 1024 writes, no flushes, no waits
   *    benchmarking writing to log...
        Wrote bytes to file: 524288
        Total relevant time taken: 4517683
        started_log_duration: 79876
        dry_run_duration: 101335
        writes_duration: 4437807
        max_iteration_duration: 9550
        min_iteration_duration: 3954
        last_iteration_duration: 4059
        cleaning up...
        Deleting: logs/24-03-09/12:09:14.txt

   * 
   * 
   * 
   */
}
