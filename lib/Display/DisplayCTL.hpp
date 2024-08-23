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
  int getIndex(int x, int y);
  CRGB *getLeds() { return leds; }
  void setupLeds();

 public:
  DisplayCTL(int width, int height, uint8_t pin, DCTLFormat format = VERTICAL);
  ~DisplayCTL() { delete[] leds; }

  DisplayCTL(const DisplayCTL &) = delete;
  DisplayCTL &operator=(const DisplayCTL &) = delete;

  int getWidth() { return width; }
  int getHeight() { return height; }

  void setPixel(int x, int y, CRGB color);
};

#endif  // DISPLAYCTL_H