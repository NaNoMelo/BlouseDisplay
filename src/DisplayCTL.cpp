#include "DisplayCTL.h"

#include <FastLED.h>

DisplayCTL::DisplayCTL(int width, int height, uint8_t pin,
                       int format = VERTICAL) {
  width = width;
  height = height;
  format = format;
  pin = pin;

  leds = new CRGB[width * height];
}

void DisplayCTL::updateLeds(CRGB **matrice) {
  int index;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      index = getIndex(i, j);
      leds[index] = matrice[i][j];
    }
  }
}

int DisplayCTL::getIndex(int x, int y) {
  return (x + x % 2) * height + (1 - 2 * x % 2) * y - x % 2;
}

/**
 * @brief Set up the leds toward FastLED library
 * @note This function exists because of the unability to use the
 * FastLED.addLeds function with a variable pin number
 */
void DisplayCTL::setupLeds() {
  switch (pin) {
    case 1:
      FastLED.addLeds<NEOPIXEL, 1>(leds, width * height, offset);
      break;
    case 2:
      FastLED.addLeds<NEOPIXEL, 2>(leds, width * height, offset);
      break;
    case 3:
      FastLED.addLeds<NEOPIXEL, 3>(leds, width * height, offset);
      break;
    case 4:
      FastLED.addLeds<NEOPIXEL, 4>(leds, width * height, offset);
      break;
    case 5:
      FastLED.addLeds<NEOPIXEL, 5>(leds, width * height, offset);
      break;
    case 6:
      FastLED.addLeds<NEOPIXEL, 6>(leds, width * height, offset);
      break;
    case 7:
      FastLED.addLeds<NEOPIXEL, 7>(leds, width * height, offset);
      break;
    case 8:
      FastLED.addLeds<NEOPIXEL, 8>(leds, width * height, offset);
      break;
    case 9:
      FastLED.addLeds<NEOPIXEL, 9>(leds, width * height, offset);
      break;
    case 10:
      FastLED.addLeds<NEOPIXEL, 10>(leds, width * height, offset);
      break;
    case 11:
      FastLED.addLeds<NEOPIXEL, 11>(leds, width * height, offset);
      break;
    case 12:
      FastLED.addLeds<NEOPIXEL, 12>(leds, width * height, offset);
      break;
    case 13:
      FastLED.addLeds<NEOPIXEL, 13>(leds, width * height, offset);
      break;
    case 14:
      FastLED.addLeds<NEOPIXEL, 14>(leds, width * height, offset);
      break;
    case 15:
      FastLED.addLeds<NEOPIXEL, 15>(leds, width * height, offset);
      break;
    case 16:
      FastLED.addLeds<NEOPIXEL, 16>(leds, width * height, offset);
      break;
    case 17:
      FastLED.addLeds<NEOPIXEL, 17>(leds, width * height, offset);
      break;
    case 18:
      FastLED.addLeds<NEOPIXEL, 18>(leds, width * height, offset);
      break;
    case 19:
      FastLED.addLeds<NEOPIXEL, 19>(leds, width * height, offset);
      break;
    case 20:
      FastLED.addLeds<NEOPIXEL, 20>(leds, width * height, offset);
      break;
    case 21:
      FastLED.addLeds<NEOPIXEL, 21>(leds, width * height, offset);
      break;
    case 22:
      FastLED.addLeds<NEOPIXEL, 22>(leds, width * height, offset);
      break;
    case 23:
      FastLED.addLeds<NEOPIXEL, 23>(leds, width * height, offset);
      break;
    case 24:
      FastLED.addLeds<NEOPIXEL, 24>(leds, width * height, offset);
      break;
  }
}