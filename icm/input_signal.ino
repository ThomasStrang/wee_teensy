#define INPUT_SIGNAL_PIN 33
#define TRIGGER_ON_RISING_EDGE true
#define TRIGGERS_PER_ROTATION 30

void setup_input_signal() {
  pinMode(INPUT_SIGNAL_PIN, INPUT);
}

bool currently_high = false;
unsigned long micros_last_signal = 0;
unsigned long micros_last_signals[TRIGGERS_PER_ROTATION];


int triggers_since_update = 0;



void handle_input_signal() {
  bool signal_high = digitalRead(INPUT_SIGNAL_PIN);
  if(signal_high != currently_high) {
    currently_high = signal_high;
    if(signal_high == TRIGGER_ON_RISING_EDGE) {
      unsigned long now = micros();
      unsigned long time_since_last_signal = now - micros_last_signal;
      micros_last_signal = now;
      micros_last_signals[triggers_since_update] = time_since_last_signal;
      triggers_since_update ++;
      if(triggers_since_update == TRIGGERS_PER_ROTATION) {
        unsigned long total_micros = 0;
        for(int i = 0; i < TRIGGERS_PER_ROTATION; i++) {
          Serial.print(micros_last_signals[i]);
          Serial.print(" ");
          total_micros += micros_last_signals[i];
        }
        Serial.println(" rotation complete.");
        Serial.print(60000000.0/((float)total_micros), 4);
        Serial.println(" rpm");
        triggers_since_update=0;
      }
    }
  }
}
