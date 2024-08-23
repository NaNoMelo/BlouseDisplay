#include "DisplayCTL.hpp"

#include <FastLED.h>

DisplayCTL::DisplayCTL(int width, int height, uint8_t pin, DCTLFormat format) {
  this->width = width;
  this->height = height;
  this->format = format;
  this->pin = pin;

  leds = new CRGB[width * height]{0, 0, 0};
  setupLeds();
}

int DisplayCTL::getIndex(int x, int y) {
  return (x + x % 2) * height + (1 - 2 * (x % 2)) * y - x % 2;
}

/**
 * @brief Set up the leds toward FastLED library
 * @note This function exists because of the unability to use the
 * FastLED.addLeds function with a variable pin number
 */
void DisplayCTL::setupLeds() {
  switch (pin) {
    case 25:
      FastLED.addLeds<NEOPIXEL, 25>(leds, width * height);
      break;
    case 27:
      FastLED.addLeds<NEOPIXEL, 27>(leds, width * height);
      break;
    default:
      break;
  }
}

void DisplayCTL::setPixel(int x, int y, CRGB color) {
  if (x < 0 || x >= width || y < 0 || y >= height)
    return;  // throw "DisplayCTL : Out of bounds setPixel";
  leds[getIndex(x, y)] = color;
}