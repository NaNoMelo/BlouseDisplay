#include "DisplayCTL.hpp"

DisplayCTL::DisplayCTL(int width, int height, uint8_t pin, DCTLFormat format) {
  _width = width;
  _height = height;
  _pin = pin;
  _format = format;

  _leds = new CRGB[width * height]{0, 0, 0};
  setupLeds();
}

CRGB DisplayCTL::getPixel(int x, int y) {
  if (x < 0 || x >= _width || y < 0 || y >= _height)
    return CRGB::Black;  // throw "DisplayCTL : Out of bounds getPixel";
  return _leds[getIndex(x, y)];
}

void DisplayCTL::setPixel(int x, int y, CRGB color) {
  if (x < 0 || x >= _width || y < 0 || y >= _height)
    return;  // throw "DisplayCTL : Out of bounds setPixel";
  _leds[getIndex(x, y)] = color;
}

int DisplayCTL::getIndex(int x, int y) {
  return (x + x % 2) * _height + (1 - 2 * (x % 2)) * y - x % 2;
}

/**
 * @brief Set up the leds toward FastLED library
 * @note This function exists because of the unability to use the
 * FastLED.addLeds function with a variable pin number
 */
void DisplayCTL::setupLeds() {
  switch (_pin) {
    case 25:
      FastLED.addLeds<NEOPIXEL, 25>(_leds, _width * _height);
      break;
    case 27:
      FastLED.addLeds<NEOPIXEL, 27>(_leds, _width * _height);
      break;
    default:
      break;
  }
}