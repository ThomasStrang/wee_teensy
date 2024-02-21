#include <SPI.h>
#include <SD.h>

File myFile;

void setup_sd_card() {
  Serial.print("\nInitializing SD card...");
  SPI.setMISO(33); 
  SPI.setMOSI(34);
  SPI.setSCLK(32);
  SD.begin(25);
}

void save_file() {
  myFile.close();
  myFile = SD.open("test.txt", FILE_WRITE);
  if(myFile) {
  } else {
    Serial.println("Error opening test.txt in write mode. Terminating.");
    while(1);
  }
}

void setup_storage_file() {
   myFile = SD.open("test.txt");
   if (myFile) {
    Serial.println("Currently persisted data on sd card: ");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    save_file();
  } else {
    Serial.println("Error opening test.txt. Terminating.");
    while(1);
  }
}

void handle_writing_to_file() {
  bool was_written_to = false;
  while(Serial.available()>0) {
    myFile.write(Serial.read());
    was_written_to = true;
  }
  if(was_written_to) {
    save_file();
    Serial.println("Wrote to file.");
  }
}
