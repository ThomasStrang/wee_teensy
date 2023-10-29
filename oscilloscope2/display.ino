

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>






class Drawable {
  protected:
    uint16_t x;
    uint16_t y;
    Adafruit_SharpMem* tft;
    
  public:
    Drawable(Adafruit_SharpMem* tft, uint16_t x, uint16_t y) : tft{tft}, x{x}, y{y} {}
    void draw();
};

class StringDrawer : public Drawable {
  protected:
    char* str;//own's it's own string, frees it once it's done.
    uint16_t num_chars;

  
  public:
    StringDrawer(Adafruit_SharpMem* tft, uint16_t x, uint16_t y, uint16_t num_chars);
    ~StringDrawer();

    void _draw(uint16_t colour);
    void update_string(char* new_str);
    void draw();
};

class FloatDrawer : public StringDrawer {
  private:
    float currently_drawn_val;
    float max_value;
    uint16_t num_decimal_places;
    char* new_str;

  public:
    FloatDrawer(Adafruit_SharpMem* tft, uint16_t x, uint16_t y, uint16_t num_chars);//num_chars can't be greater than 9.
    ~FloatDrawer();

    void update_float(float f);
    void draw();
};








Adafruit_SharpMem display(&SPI, 10, 400, 240, 5000000);
char* zero = '0-';
StringDrawer zero_drawer = StringDrawer(&display,150,10,2);
FloatDrawer scale_drawer = FloatDrawer(&display,200,10,6);
FloatDrawer freq_drawer = FloatDrawer(&display,10,10,12);
FloatDrawer max_drawer = FloatDrawer(&display,300,10,7);

void setup_display() {
  display.begin();
  display.clearDisplay();
}

uint16_t input_to_graph_y(uint16_t i) {
  return ((i*NUM_Y_PIXELS_IN_GRAPH)/current_y_scale)-20;
}

uint16_t graph_y_to_y(uint16_t i) {
  return 10+NUM_Y_PIXELS_IN_GRAPH-i;
}

uint16_t voltage_to_pixel(uint16_t v) {
  return graph_y_to_y(input_to_graph_y(v));
}

void draw_graph(uint16_t* voltages) {
  display.clearDisplay();
  uint16_t previous_pixel;
  for(int i = 0; i < NUM_VALS_IN_GRAPH; i++) {
    uint16_t current_pixel=voltage_to_pixel(voltages[i]);
    if(i>0){
      uint16_t lesser_val = min(previous_pixel,current_pixel);
      uint16_t greater_val = max(previous_pixel,current_pixel);
      display.drawFastVLine(10+i,lesser_val,greater_val-lesser_val, BLACK);
    } else {
      display.drawPixel(10+i,current_pixel, BLACK);
    }
    previous_pixel=current_pixel;
  }
}

uint16_t get_max(uint16_t* voltages) {
  uint16_t mx = 0;
  for(int i = 0; i < NUM_VALS_IN_GRAPH; i++) {
    if(voltages[i]>mx)
      mx=voltages[i];
  }
  return mx;
}




StringDrawer::StringDrawer(Adafruit_SharpMem* tft, uint16_t x, uint16_t y, uint16_t num_chars): Drawable(tft,x,y), num_chars{num_chars} {
  str=(char*) calloc(num_chars,sizeof(char));
}
StringDrawer::~StringDrawer() {
  free(str);
}

void StringDrawer::_draw(uint16_t colour) {
  tft->setCursor(x,y);
  tft->setTextWrap(true);
  tft->setTextSize(2);
  tft->setTextColor(colour);
  tft->print(str);
}

void StringDrawer::update_string(char* new_str) {
  for(int i=0; i<num_chars; i++) {
    if(new_str[i]!=str[i]) {
      for(int j=0; j<num_chars; j++) {
        str[j]=new_str[j];
      }
      draw();
      break;
    }
  }
}

void StringDrawer::draw() {
  _draw(BLACK);
}


FloatDrawer::FloatDrawer(Adafruit_SharpMem* tft, uint16_t x, uint16_t y, uint16_t num_chars ) : StringDrawer(tft,x,y,num_chars), currently_drawn_val{0.0} {
  new_str=(char*) calloc(num_chars,sizeof(char));
  max_value=pow(10,num_chars)-1;
}

FloatDrawer::~FloatDrawer() {
  free(new_str);
}

void FloatDrawer::update_float(float f) {
  if(f>max_value) {
    f=max_value;
  }
  snprintf(new_str, num_chars*sizeof(char), "%6.2f", f);
  update_string(new_str);
}

void FloatDrawer::draw() {
  _draw(BLACK);
}

void draw_legend(uint16_t* voltages) {
  float max_voltage = get_max(voltages) * MAX_VOLTAGE / 255.0;
  max_drawer.update_float((float) max_voltage);
  max_drawer.draw();
  display.print("V");
  float scale_voltage = current_y_scale* MAX_VOLTAGE / 255.0;
  scale_drawer.update_float((float) scale_voltage);
  scale_drawer.draw();
  display.print("V");

  freq_drawer.update_float(current_reading_frequency);
  freq_drawer.draw();
  display.print(" HZ");
}

void draw(uint16_t* voltages) {
  draw_graph(voltages);
  draw_legend(voltages);
  display.refresh();
}
