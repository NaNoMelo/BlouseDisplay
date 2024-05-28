#ifndef DISPLAYCTL_H
#define DISPLAYCTL_H

#include <Arduino.h>
#include <FastLED.h>

enum DCTLFormat { VERTICAL, HORIZONTAL };
class DisplayCTL {
 private:
  int width;
  int height;
  uint8_t pin;
  DCTLFormat format = VERTICAL;
  CRGB *leds;
  int offset = 0;

 protected:
  friend class DisplayAssembly;
  int getIndex(int x, int y);
  void setOffset(int offset) { this->offset = offset; }
  // void setupLeds();

 public:
  int getWidth() { return width; }
  int getHeight() { return height; }
  int getOffset() { return offset; }
  CRGB *getLeds() { return leds; }
  DisplayCTL(int width, int height, uint8_t pin, DCTLFormat format = VERTICAL);
  ~DisplayCTL() { delete[] leds; }
};

#endif  // DISPLAYCTL_H