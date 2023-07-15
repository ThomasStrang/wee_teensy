
const int LED =  13;

void setup()   {                
  pinMode(LED, OUTPUT);
} 

int light_val = 0;
void loop()                     
{
  if(light_val == 255) {
    light_val = 0;
  } else {
    light_val+=5;
  }
  analogWrite(LED, light_val);
  delay(50);
}
