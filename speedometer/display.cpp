#include "display.h"
#include <Arduino.h>

uint16_t parse_colour(uint16_t r, uint16_t g, uint16_t b) {
  return (b<<11)+(g*2<<5)+r;
}

uint16_t COLOUR_BLACK = parse_colour(0,0,0);
uint16_t COLOUR_RED = parse_colour(18,0,0);
uint16_t COLOUR_REDDER = parse_colour(31,0,0);
uint16_t COLOUR_LIME = parse_colour(4,16,24);
uint16_t COLOUR_WHITE = parse_colour(31,31,31);
uint16_t COLOUR_PIKACHU = parse_colour(30,28,0);
uint16_t COLOUR_PIKACHU_LIGHT = parse_colour(31,29,15);
uint16_t COLOUR_BACKGROUND = COLOUR_BLACK;



StringDrawer::StringDrawer(Adafruit_ST7735* tft, uint8_t x, uint8_t y, uint8_t num_chars): Drawable(tft,x,y), num_chars{num_chars} {
  str=(char*) calloc(num_chars,sizeof(char));
}
StringDrawer::~StringDrawer() {
	free(str);
}

void StringDrawer::_draw(uint16_t colour) {
	tft->setCursor(x,y);
	tft->setTextWrap(true);
	tft->setTextSize(3);
	tft->setTextColor(colour);
	tft->print(str);
}

void StringDrawer::update_string(char* new_str) {
	for(int i=0; i<num_chars; i++) {
		if(new_str[i]!=str[i]) {
			undraw();
			for(int j=0; j<num_chars; j++) {
				str[j]=new_str[j];
			}
			draw();
			break;
		}
	}
}

void StringDrawer::draw() {
	_draw(COLOUR_WHITE);
}

void StringDrawer::undraw() {
	_draw(COLOUR_BACKGROUND);
}

FloatDrawer::FloatDrawer(Adafruit_ST7735* tft, uint8_t x, uint8_t y, uint8_t num_chars ) : StringDrawer(tft,x,y,num_chars), currently_drawn_val{0.0} {
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
	snprintf(new_str, num_chars*sizeof(char), "%5.2f", f);
  update_string(new_str);
}

void FloatDrawer::draw() {
	_draw(COLOUR_WHITE);
} 

void FloatDrawer::undraw() {
	_draw(COLOUR_BACKGROUND);
} 

GraphDrawer::GraphDrawer(Adafruit_ST7735* tft, uint8_t x, uint8_t y, uint8_t num_x_pixels, uint8_t num_y_pixels, float x_scale, float y_scale, char* title) : Drawable(tft,x,y), num_x_pixels{num_x_pixels}, num_y_pixels{num_y_pixels}, x_scale{x_scale}, y_scale{y_scale}, title{title} {
	pixels=(uint8_t*) calloc(num_x_pixels,sizeof(uint8_t));
	internal_y=y+num_y_pixels-1;
}

GraphDrawer::~GraphDrawer() {free(pixels);}

void GraphDrawer::draw_title(uint16_t colour) {
	tft->setCursor(x+2, y+2);
	tft->setTextColor(colour);
	tft->setTextWrap(true);
	tft->print(title);
}

void GraphDrawer::draw_border(uint16_t colour) {
	tft->drawFastHLine(x-2, y-2, num_x_pixels+4, colour);
	tft->drawFastHLine(x-2, y+num_y_pixels+2, num_x_pixels+4, colour);
	tft->drawFastVLine(x-2, y-2, num_y_pixels+4, colour);
	tft->drawFastVLine(x+num_x_pixels+2, y-2, num_y_pixels+4, colour);
}

void GraphDrawer::draw_border() {
	draw_border(COLOUR_RED);
}

uint8_t GraphDrawer::graph_y_to_display_y(uint8_t y) {
	return internal_y-y;
}

uint8_t GraphDrawer::graph_x_to_display_x(uint8_t graph_x) {
	return x+graph_x;
}

void GraphDrawer::draw() {
	for(uint8_t x=0; x<num_x_pixels; x++) {
		tft->drawPixel(graph_x_to_display_x(x), graph_y_to_display_y(pixels[x]), COLOUR_WHITE);
	}
	draw_border();
	draw_title(COLOUR_PIKACHU);
}

void GraphDrawer::undraw() {
	for(uint8_t x=0; x<num_x_pixels; x++) {
		tft->drawPixel(graph_x_to_display_x(x), graph_y_to_display_y(pixels[x]), COLOUR_BACKGROUND);
	}
	draw_border(COLOUR_BACKGROUND);
	draw_title(COLOUR_BACKGROUND);
}

void GraphDrawer::move_graph_and_draw(uint8_t new_x, uint8_t new_y) {
	undraw();
	x = new_x;
	y = new_y;
	internal_y=new_y+num_y_pixels-1;
	draw();
}

uint8_t GraphDrawer::x_val_to_pixel_num(float x) {
	float x_fraction_of_graph=x/x_scale;
	uint8_t return_val=(uint8_t) ((int)round((x_fraction_of_graph * (float)num_x_pixels)) % num_x_pixels);
	return return_val;
}

uint8_t GraphDrawer::y_val_to_pixel_num(float y) {
	float y_fraction_of_graph=y/y_scale;
	uint8_t return_val=(uint8_t) (((int)round((y_fraction_of_graph * (float)num_y_pixels))) % num_y_pixels);
	return return_val;
}

void GraphDrawer::draw_pixel(uint8_t x_pixel, uint8_t y_pixel) {
	if(pixels[x_pixel]!=y_pixel) {
		tft->drawPixel(graph_x_to_display_x(x_pixel), graph_y_to_display_y(pixels[x_pixel]), COLOUR_BACKGROUND);
		tft->drawPixel(graph_x_to_display_x(x_pixel), graph_y_to_display_y(y_pixel), COLOUR_WHITE);
		pixels[x_pixel]=y_pixel;
	}
}

void GraphDrawer::draw_xy(float x, float y) {
	uint8_t x_pixel=x_val_to_pixel_num(x);
	uint8_t y_pixel=y_val_to_pixel_num(y);
	this->draw_pixel(x_pixel,y_pixel);
}

void GraphDrawer::move_pixels_over() {
	for(int i=0; i<num_x_pixels-1; i++) {
		this->draw_pixel(i, pixels[i+1]);
	}
}

void GraphDrawer::add_val_to_end(float y) {
	this->move_pixels_over();
	this->draw_pixel(num_x_pixels-1, y_val_to_pixel_num(y));
}

void Sprite::draw() {
	for(uint16_t i=0; i<num_pixels; i++){
		byte red=image_buffer[i*4]>>3;
		byte green=image_buffer[i*4+1]>>3;
		byte blue=image_buffer[i*4+2]>>3;
		byte gamma=image_buffer[i*4+3]>>3;
		uint16_t row=i>>6;
		if(gamma) {
			tft->drawPixel(x+i%64,y+row,parse_colour(red,green,blue));//TODO parse this once, throw out the unneccessarily large buffer, keep the parsed
		}
	}
}

void Sprite::undraw() {
	for(uint16_t i=0; i<num_pixels; i++){
		byte gamma=image_buffer[i*4+3]>>3;
		uint16_t row=i>>6;
		if(gamma) {
			tft->drawPixel(x+i%64,y+row,COLOUR_BACKGROUND);//TODO parse this once, throw out the unneccessarily large buffer, keep the parsed
		}
	}
}