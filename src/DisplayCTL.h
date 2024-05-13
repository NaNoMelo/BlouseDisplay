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

 protected:
  friend class DisplayAssembly;
  int getIndex(int x, int y);
  int offset = 0;
  void setupLeds();

 public:
  int getWidth() { return width; }
  int getHeight() { return height; }
  DisplayCTL(int width, int height, uint8_t pin, int format = VERTICAL);
  void updateLeds(CRGB **matrice);
  ~DisplayCTL() { delete[] leds; }
};

#endif  // DISPLAYCTL_H