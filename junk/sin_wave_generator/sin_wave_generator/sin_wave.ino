
bool SIN_WAVE_ENABLED = false;
int DAC_PIN = 10;

void setup_sin_wave() {
  if(SIN_WAVE_ENABLED) {
    pinMode(LED1, OUTPUT);
    digitalWrite(LED1, HIGH);
    pinMode(DAC_PIN, OUTPUT);
    analogWrite(DAC_PIN, 128);
  }
}
double AC_SIGNAL_FREQUENCY = 500.0; // hz

double micros_per_signal = 1000000.0d/AC_SIGNAL_FREQUENCY;
double micros_per_radian_of_sin = micros_per_signal / (2.0d*PI);

int num_signals_completed(long current_micros) {
  return (int) (current_micros/micros_per_signal);
}

bool current_signal_should_be_skipped(int current_signal_num) {
  //we assume that the eccentric shaft has 36 teeth, but that 5 teeth are missing:
  current_signal_num = current_signal_num %36;
  return current_signal_num==13 || current_signal_num==14 || current_signal_num==16 || current_signal_num==17 || current_signal_num==34 || current_signal_num==35;
}

void handle_sin_wave() {
  if(SIN_WAVE_ENABLED) {
    long num_micros_passed = micros();
    int current_signal_num = num_signals_completed(num_micros_passed);
    bool should_skip = current_signal_should_be_skipped(current_signal_num);
    if(should_skip) {
      analogWrite(DAC_PIN,127);
    } else {
      double num_radians_passed = num_micros_passed / micros_per_radian_of_sin;
      double output_signal = (sin(num_radians_passed)+1.0d)*128.0d;
      analogWrite(DAC_PIN,output_signal);
    }
  } else {
    delay(100);
  }
}

void toggle_sin_wave() {
  SIN_WAVE_ENABLED=true;
}
