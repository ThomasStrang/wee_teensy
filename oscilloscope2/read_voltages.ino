
#define INPUT_PIN 15

#include <ADC.h>
#include <ADC_util.h>

ADC adc = ADC();
uint16_t voltages[NUM_VALS_IN_GRAPH];

uint16_t read_voltage() {
  delayMicroseconds(wait_micros);
  while(!adc.adc0->isComplete()){}
  return adc.adc0->analogReadContinuous();
}

uint16_t* read_voltages() {
  adc.adc0->startContinuous(INPUT_PIN);
  long start = micros();
  for(int i = 0; i < NUM_VALS_IN_GRAPH; i++) {
    voltages[i] = read_voltage();
  }
  long taken = micros()-start;
  adc.adc0->stopContinuous();
  float micros_per_reading = (float) taken /(float)(NUM_VALS_IN_GRAPH);
  current_reading_frequency = 1000000/micros_per_reading;
  Serial.print("Reading voltages at this frequency: ");
  Serial.println(current_reading_frequency);
  return voltages;
}

void sync_with_signal() {
  int now = micros();
  pinMode(INPUT_PIN,INPUT);
  while(digitalRead(INPUT_PIN)) 
    if(micros() - now > 1000) 
      break;
  while(!digitalRead(INPUT_PIN)) 
    if(micros() - now > 1000) 
      break;
  pinMode(INPUT_PIN,INPUT_DISABLE);
}

void setup_voltage_reading() {
  adc.adc0->setAveraging(0);
  adc.adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED );
  adc.adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_VERY_HIGH_SPEED);
  adc.adc0->setResolution(8);
}
