#include <Arduino.h>
#include <Preferences.h>
#include <stdlib.h>
#include <time.h>

#include "DisplayAssembly.hpp"
#include "DisplayCTL.hpp"
#include "background.h"
#include "overlay.h"

Preferences preferences;
DisplayAssembly display;
CRGB ***matrice = display.getMatrice();

#define BG_BUTTON_PIN 19
#define MODE_BUTTON_PIN 18

void matriceToLed();
void buildMatrice();

void setup() {
  Serial.begin(115200);
  preferences.begin("display");
  DisplayCTL *top = new DisplayCTL(32, 8, 27, VERTICAL);
  DisplayCTL *bottom = new DisplayCTL(32, 8, 25, VERTICAL);
  display.addController(top, 0, 0);
  display.addController(bottom, 0, 8);
  display.updateMatrice();
  FastLED.addLeds<NEOPIXEL, 27>(top->getLeds(),
                                top->getWidth() * top->getHeight());
  FastLED.addLeds<NEOPIXEL, 25>(bottom->getLeds(),
                                bottom->getWidth() * bottom->getHeight());

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  FastLED.setMaxRefreshRate(30);
  FastLED.setBrightness(1);
  srand(time(NULL));
  pinMode(BG_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  Serial.println("loop");
  static short bg = preferences.getShort("bg", 2);
  static short brightness = preferences.getShort("brightness", 1);
  static long counter = 0;
  static bool button_bg = false;
  static bool button_brightness = false;
  if (!digitalRead(BG_BUTTON_PIN)) {
    if (!button_bg) {
      button_bg = true;
      bg++;
      if (bg > 4) bg = 1;
    }
  } else {
    if (button_bg) {
      preferences.putShort("bg", bg);
      button_bg = false;
    }
  }

  if (!digitalRead(MODE_BUTTON_PIN)) {
    if (!button_brightness) {
      button_brightness = true;
      brightness++;
      if (brightness > 1) brightness = 0;
    }
  } else {
    if (button_brightness) {
      preferences.putShort("brightness", brightness);
      button_brightness = false;
    }
  }
  brightness ? FastLED.setBrightness(1) : FastLED.setBrightness(16);

  if (!(counter % 5)) {
    switch (bg) {
      case 1:
        matriceRgb(display, brightness);
        break;
      case 2:
        matrix(display);
        break;
      case 3:
        fire(display);
        break;
      case 4:
        epilepsie(display);
        break;
      default:
        break;
    }
  }

  if (!(counter % 5)) {
    FastLED.show();
  }
  delay(10);
  counter++;
}