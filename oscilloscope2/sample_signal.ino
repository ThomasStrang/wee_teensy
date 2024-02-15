int pwm_frequency = 5000;
int pwm_signal = 0;

void setup_signal() {
  analogWriteFrequency(3, pwm_frequency);
  analogWrite(3, pwm_signal);
}

void update_signal() {
  if (pwm_signal >= 256) {
    pwm_signal = 0;
  } else {
    pwm_signal += 1;
  }
  //  pwm_signal=100;
  analogWrite(3, pwm_signal);
}
