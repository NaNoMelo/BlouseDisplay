#ifndef DISPLAYCTL_H
#define DISPLAYCTL_H

#include <Arduino.h>
#include <FastLED.h>

enum DCTLFormat { VERTICAL, HORIZONTAL };
class DisplayCTL {
 public:
  DisplayCTL(int width, int height, uint8_t pin, DCTLFormat format = VERTICAL);
  ~DisplayCTL() { delete[] _leds; }

  DisplayCTL(const DisplayCTL &) = delete;
  DisplayCTL &operator=(const DisplayCTL &) = delete;

  int getWidth() { return _width; }
  int getHeight() { return _height; }

  void setPixel(int x, int y, CRGB color);

 protected:
  int getIndex(int x, int y);
  CRGB *getLeds() { return _leds; }
  void setupLeds();

 private:
  int _width;
  int _height;
  uint8_t _pin;
  DCTLFormat _format = VERTICAL;
  CRGB *_leds;
};

#endif  // DISPLAYCTL_H