#include <stdint.h>
#include <Adafruit_ST7735.h>

struct Position {
    int16_t x;
    int16_t y;
    Position(int16_t x, int16_t y) {
      this->x=x;
      this->y=y;
    }
    Position() {
      x=0;
      y=0;
    }

    Position copy() {
      return Position(x, y);
    }
};

inline bool operator==(const Position& a, const Position& b) {
  return a.x==b.x && a.y==b.y;
}

inline bool operator!=(const Position& a, const Position& b) {
  return !(a==b);
}

class Sprite {
  public:
    Position p=Position();
    Position drawn_p=Position();
    bool viewable=false;
    bool viewed=false;
    Adafruit_ST7735* tft;

    Sprite(Adafruit_ST7735* t) {
      tft=t;
    }
    
    virtual void _draw()=0;
    virtual void _undraw()=0;
    
    void draw() {
      if(viewable) {
        if(viewed) {
          if(drawn_p!=p) {
            _undraw();
            _draw();
            drawn_p=p.copy();
          }
        } else {
          _draw();
          drawn_p=p.copy();
          viewed=true;
        }
      } else if(viewed) {
        _undraw();
        viewed=false;
      }
    }
};

class Square : public Sprite {
  public:
    uint16_t colour = ST77XX_CYAN;
  
    Square(Adafruit_ST7735* t): Sprite(t) {}
    
    void _draw() {
      tft->fillRect(p.x,p.y,12,12,colour);
    }
  
    void _undraw() {
      tft->fillRect(drawn_p.x,drawn_p.y,12,12,0x0000);
    }
};
