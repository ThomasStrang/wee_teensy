#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include "sd_card.h"

void setup_sd_card() {
  SPI.setMISO(33); 
  SPI.setMOSI(34);
  SPI.setSCLK(32);
  SD.begin(25);
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
  for(int i=filename.length()-1;i>=0;i--) {
    if(filename.charAt(i)=='/') {
      name_of_file[i]='\0';
      bool directory_exists = SD.exists(String(name_of_file));
      name_of_file[i]='/';
      if(!directory_exists) {
        print_invalid_filename();
        return;
      } else {
        break;
      }
    }
  }
  if(SD.exists(filename)) {
    print_invalid_filename();
    return;
  }
  File f = SD.open(filename, FILE_WRITE);
  if(f) {
    f.close(); 
  } else {
    print_failure();
  }
}

void make_directory(char* name_of_directory) {
  String filename = String(name_of_directory);
  if(SD.exists(filename)) {
    print_invalid_filename();
    return;
  }
  if(!SD.mkdir(filename)) {
    print_failure();
  }
}


void rm_file(char* name_of_file) {
  String filename = String(name_of_file);
  if(!SD.exists(filename)) {
    print_invalid_filename();
    return;
  }
  if(!SD.remove(filename)) {
    print_failure();
  }
}

void rm_directory(char* name_of_directory) {
  String filename = String(name_of_directory);
  if(!SD.exists(filename)) {
    print_invalid_filename();
    return;
  }
  if(!SD.rmdir(filename)) {
    print_failure();
  }
}

void print_file_contents(char* name_of_file) {
  String filename = String(name_of_file);
  if(!SD.exists(filename)) {
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
