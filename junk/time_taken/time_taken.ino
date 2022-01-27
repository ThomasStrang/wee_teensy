#define MAX_ARRAY_SIZE 1000
#define MIN_ARRAY_SIZE 4
int array_size = MIN_ARRAY_SIZE;

int an_array[MAX_ARRAY_SIZE];

void setup() {
  // put your setup code here, to run once:
  while (!Serial) {
    // wait for Arduino Serial Monitor to be ready
  }
}

void randomise_array() {
  for(int i = 0; i < array_size; i++) {
    an_array[i] = random(0, array_size);
  }
}

void bubble_sort() {
  for(int check_len = array_size; check_len > 1; check_len--) { 
    for(int i = 0; i < check_len-1; i++) {
      int first = an_array[i];
      int second = an_array[i+1];
      if(first > second) {
        an_array[i] = second;
        an_array[i+1] = first;
      }
    }
  }
}

void loop() {
  randomise_array();
  int start_time = micros();
  bubble_sort();
  
  Serial.print("\nArray size: ");
  Serial.print(array_size);
  Serial.print(" micros taken: ");
  Serial.print(micros()-start_time);
  array_size++;
  if(array_size > MAX_ARRAY_SIZE) array_size = MIN_ARRAY_SIZE;
  delay(100);//wait a second or two
}
