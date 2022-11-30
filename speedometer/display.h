#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>


extern uint16_t COLOUR_BLACK;
extern uint16_t COLOUR_RED;
extern uint16_t COLOUR_REDDER;
extern uint16_t COLOUR_LIME;
extern uint16_t COLOUR_WHITE;
extern uint16_t COLOUR_PIKACHU;
extern uint16_t COLOUR_PIKACHU_LIGHT;
extern uint16_t COLOUR_BACKGROUND;
extern uint16_t parse_colour(uint16_t r, uint16_t g, uint16_t b);

class Drawable {
  protected:
    uint8_t x;
    uint8_t y;
    Adafruit_ST7735* tft;
    
  public:
    Drawable(Adafruit_ST7735* tft, uint8_t x, uint8_t y) : tft{tft}, x{x}, y{y} {}
    void draw();
    void undraw();
};

class StringDrawer : Drawable {
  protected:
    char* str;//own's it's own string, frees it once it's done.
    uint8_t num_chars;

  
  public:
    StringDrawer(Adafruit_ST7735* tft, uint8_t x, uint8_t y, uint8_t num_chars);
    ~StringDrawer();

    void _draw(uint16_t colour);
    void update_string(char* new_str);
    void draw();
    void undraw();
};

class FloatDrawer : StringDrawer {
  private:
    float currently_drawn_val;
    float max_value;
    uint8_t num_decimal_places;
    char* new_str;

  public:
    FloatDrawer(Adafruit_ST7735* tft, uint8_t x, uint8_t y, uint8_t num_chars);//num_chars can't be greater than 9.
    ~FloatDrawer();

    void update_float(float f);
    void draw();
    void undraw();
};

class GraphDrawer : Drawable {
  private:
    uint8_t internal_y;
    uint8_t num_x_pixels;
    uint8_t num_y_pixels;
    uint8_t* pixels;//the y position of each x pixel. this array is owned by this object and so is freed post deletion
    float y_scale=100;
    float x_scale=100;
    char* title;
 
  public:
    GraphDrawer(Adafruit_ST7735* tft, uint8_t x, uint8_t y, uint8_t num_x_pixels, uint8_t num_y_pixels, float x_scale, float y_scale, char* title);
    ~GraphDrawer();

    void draw();
    void undraw();
    void draw_title(uint16_t colour);
    void draw_border(uint16_t colour);
    void draw_border();
    uint8_t graph_y_to_display_y(uint8_t y);
    uint8_t graph_x_to_display_x(uint8_t graph_x);
    void move_graph_and_draw(uint8_t new_x, uint8_t new_y);
    uint8_t x_val_to_pixel_num(float x);
    uint8_t y_val_to_pixel_num(float y);
    void draw_pixel(uint8_t x_pixel, uint8_t y_pixel);
    void draw_xy(float x, float y);
    void move_pixels_over();
    void add_val_to_end(float y);
};

class Sprite : Drawable {
  private:
    int num_pixels;
    byte* image_buffer;
    
  public:
    Sprite(Adafruit_ST7735* tft, int num_pixels, byte* image_buffer, uint16_t x, uint16_t y) : Drawable(tft,x,y), num_pixels{num_pixels}, image_buffer{image_buffer} {}

    void draw();
    void undraw();
};
#endif