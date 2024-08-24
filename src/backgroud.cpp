#include <Arduino.h>

#include "DisplayAssembly.hpp"
#include "background.hpp"

void matriceRgb(DisplayAssembly *display, int mode) {
  const CRGB rgb[] = {CRGB::Red,  CRGB::Yellow, CRGB::Green,
                      CRGB::Cyan, CRGB::Blue,   CRGB::Magenta};
  static short int hue = 0;
  for (int x = 0; x < display->getWidth(); x++) {
    for (int y = 0; y < display->getHeight(); y++) {
      if (mode) {
        display->setPixel(x, y, rgb[(hue + x + y) / 16 % 6]);
      } else {
        display->setPixel(
            x, y,
            CHSV(map(x + y + hue, 0,
                     4 * (display->getWidth() + display->getHeight()), 0, 255),
                 255, 255));
      }
    }
  }
  hue += 1;
}

void matrix(DisplayAssembly *display) {
  static const int width = display->getWidth(), height = display->getHeight();
  // WARNING constants manually set
  static CRGB storage[32][16]{0, 0, 0};
  int alea;
  for (int y = height - 1; y > 0; y--) {
    for (int x = 0; x < width; x++) {
      storage[x][y] = storage[x][y - 1];
    }
  }
  for (int x = 0; x < width; x++) {
    storage[x][0] = CRGB::Black;
  }
  for (int i = 0; i < 3; i++) {
    alea = rand() % width;
    storage[alea][0] = CRGB::Green;
  }

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      display->setPixel(x, y, storage[x][y]);
    }
  }
}

void fire(DisplayAssembly *display) {
  static int randFeu[32];
  static float etatFeu[32];
  for (int i = 0; i < display->getWidth(); i++) {
    randFeu[i] = rand() % display->getHeight();
    switch (i) {
      case 0:
        etatFeu[i] = (etatFeu[i] + randFeu[i] + randFeu[i + 1]) / 3;
        break;

      case 32 - 1:
        etatFeu[i] = (etatFeu[i] + randFeu[i - 1] + randFeu[i]) / 3;
        break;

      default:
        etatFeu[i] =
            (etatFeu[i] + randFeu[i - 1] + randFeu[i] + randFeu[i + 1]) / 4;
        break;
    }
  };

  for (int x = 0; x < display->getWidth(); x++) {
    for (int y = 0; y < display->getHeight(); y++) {
      if (y < etatFeu[x]) {
        display->setPixel(x, y, CRGB::Black);
      } else {
        display->setPixel(
            x, y,
            CHSV(map(constrain(y - etatFeu[x], 0, display->getHeight() - 1), 0,
                     display->getHeight() - 1, 0, 60),
                 255, 255));
      }
    }
  }
}

void epilepsie(DisplayAssembly *display) {
  static int hue = 0;
  const CRGB rgb[] = {CRGB::Red,  CRGB::Yellow, CRGB::Green,
                      CRGB::Cyan, CRGB::Blue,   CRGB::Magenta};
  for (int x = 0; x < display->getWidth(); x++) {
    for (int y = 0; y < display->getHeight(); y++) {
      display->setPixel(x, y, rgb[hue % 6]);
    }
  }
  hue++;
}