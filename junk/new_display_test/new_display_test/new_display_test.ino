#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>


uint16_t parse_colour(uint16_t r, uint16_t g, uint16_t b) {
  return (g<<10)+(r<<5)+(b);
}

uint16_t COLOUR_BLACK = parse_colour(0,0,0);
uint16_t COLOUR_RED = parse_colour(31,0,0);
uint16_t COLOUR_LIME = parse_colour(4,16,24);
uint16_t COLOUR_WHITE = parse_colour(31,31,31);


Adafruit_ST7735 tft = Adafruit_ST7735(&SPI1,9,8,12);


class GraphDrawer {
  public:
    uint8_t x_offset;
    uint8_t y_offset;
    uint8_t _internal_y_offset;
    uint8_t num_x_pixels;
    uint8_t num_y_pixels;
    uint8_t* pixels;//the y position of each x pixel

    float y_scale=100;
    float x_scale=100;

    char* title;
    
    GraphDrawer(uint8_t x_offset, uint8_t y_offset, uint8_t num_x_pixels, uint8_t num_y_pixels, float x_scale, float y_scale, char* title) : x_offset{x_offset}, y_offset{y_offset}, num_x_pixels{num_x_pixels}, num_y_pixels{num_y_pixels}, x_scale{x_scale}, y_scale{y_scale}, title{title} {
      pixels = (uint8_t*) calloc(num_x_pixels, sizeof(uint8_t));
      _internal_y_offset=y_offset+num_y_pixels-1;
    }
  
    ~GraphDrawer() {
      free(pixels); 
    }

    void draw_title(uint16_t colour) {
      
      tft.setCursor(x_offset+2, y_offset+2);
      tft.setTextColor(colour);
      tft.setTextWrap(true);
      tft.print(title);
      
    }

    void draw_title() {
      draw_title(COLOUR_RED);
    }

    void _undraw_title() {
      draw_title(COLOUR_BLACK);
    }

    void draw_border(uint16_t colour) {
        tft.drawFastHLine(x_offset-2, y_offset-2, num_x_pixels+4, colour);
        tft.drawFastHLine(x_offset-2, y_offset+num_y_pixels+2, num_x_pixels+4, colour);
        tft.drawFastVLine(x_offset-2, y_offset-2, num_y_pixels+4, colour);
        tft.drawFastVLine(x_offset+num_x_pixels+2, y_offset-2, num_y_pixels+4, colour);
    }

    void draw_border() {
        draw_border(COLOUR_RED);
    }

    void _undraw_border() {
        draw_border(COLOUR_BLACK);
    }

    uint8_t graph_y_to_display_y(uint8_t y) {
      return _internal_y_offset-y;
    }

    uint8_t graph_x_to_display_x(uint8_t x) {
      return x_offset+x;
    }
  
  
    void clear_graph() {
      for(uint8_t x = 0; x < num_x_pixels; x++) 
        tft.drawPixel(graph_x_to_display_x(x), graph_y_to_display_y(pixels[x]), COLOUR_BLACK);
      _undraw_border();
      _undraw_title();
    }
    
    void draw_graph() {
      for(uint8_t x = 0; x < num_x_pixels; x++) {
        tft.drawPixel(graph_x_to_display_x(x), graph_y_to_display_y(pixels[x]), COLOUR_LIME);
      }
      draw_border();
      draw_title();
    }

    void move_graph(uint8_t new_x_offset, uint8_t new_y_offset) {
      clear_graph();
      x_offset = new_x_offset;
      y_offset = new_y_offset;
      _internal_y_offset=new_y_offset+num_y_pixels-1;
      draw_graph();
    }
    
    uint8_t x_val_to_pixel_num(float x) {
      float x_fraction_of_graph = x/x_scale;
      uint8_t return_val = (uint8_t) round((x_fraction_of_graph * (float)num_x_pixels)) % num_x_pixels;
      Serial.print("converting x to pixel num: ");
      Serial.print(x);
      Serial.print(" -> ");
      Serial.print(x_fraction_of_graph);
      Serial.print(" -> ");
      Serial.print((x_fraction_of_graph * (float)num_x_pixels));
      Serial.print(" -> ");
      Serial.print(round(x_fraction_of_graph * (float)num_x_pixels));
      Serial.print(" -> ");
      Serial.print((uint8_t) round((x_fraction_of_graph * (float)num_x_pixels)));
      Serial.print(" -> ");
      Serial.println(return_val);
      return return_val;
    }

    uint8_t y_val_to_pixel_num(float y) {
      float y_fraction_of_graph = y/y_scale;
      uint8_t return_val =   (uint8_t) round((y_fraction_of_graph * (float)num_y_pixels)) % num_y_pixels;
      return return_val;
    }

    void draw(float x, float y) {
      uint8_t x_pixel=x_val_to_pixel_num(x);
      uint8_t y_pixel=y_val_to_pixel_num(y);
      Serial.print("to pixel nums: (");
      Serial.print(x_pixel);
      Serial.print(" , ");
      Serial.print(y_pixel);
      Serial.println(")");
      if(pixels[x_pixel]!=y_pixel) {
        tft.drawPixel(graph_x_to_display_x(x_pixel), graph_y_to_display_y(pixels[x_pixel]), COLOUR_BLACK);
        tft.drawPixel(graph_x_to_display_x(x_pixel), graph_y_to_display_y(y_pixel), COLOUR_LIME);
        pixels[x_pixel]=y_pixel;
      }
    }
};


char speed_title[] ="Vehicle Speed (mph)";
char tacho_title[] ="Engine Speed (rpm)";
GraphDrawer graph = GraphDrawer(20,6,120, 50, 100, 10000, speed_title);
GraphDrawer graph2 = GraphDrawer(20,72,120, 50, 100, 10000, tacho_title);
char text[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserun";


void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void setup() {
  SPI1.setCS(9);
  SPI1.setSCK(10);
  SPI1.setTX(11);
  SPI1.begin(true);
  tft.initR(INITR_GREENTAB);
  tft.setRotation(3);
  tft.fillScreen(COLOUR_BLACK);
  pinMode(13, OUTPUT);
  analogWrite(13, 32);
  graph.draw_graph();
  graph2.draw_graph();
  for(int i =0; i < 100; i++) {
    graph.draw(i,i*i);
    graph2.draw(i,i*100);
  }
}

void loop() {
  handle_input();
  delay(100);
}
