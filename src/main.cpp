#include <Arduino.h>
#include <Preferences.h>
#include <stdlib.h>
#include <time.h>

#include "Debouncer.hpp"
#include "DisplayAssembly.hpp"
#include "DisplayCTL.hpp"
#include "background.h"
#include "overlay.h"

Preferences preferences;
DisplayAssembly display;

#define FPS 30

#define BG_BUTTON_PIN 19
#define BRIGHTNESS_BUTTON_PIN 18

void setup() {
  Serial.begin(115200);
  preferences.begin("display");
  display.addController(32, 8, 27, VERTICAL, 0, 0);
  display.addController(32, 8, 25, VERTICAL, 0, 8);

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  FastLED.setMaxRefreshRate(600);
  FastLED.setBrightness(1);
  srand(time(NULL));
  pinMode(BG_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BUTTON_PIN, INPUT_PULLUP);
  FastLED.show();
}

Debouncer bg_button(BG_BUTTON_PIN);
Debouncer brightness_button(BRIGHTNESS_BUTTON_PIN);
void loop() {
  /* TEST SEGMENT --------------------
  static unsigned short hue = 0;
  long time = millis();
  for (int i = 0; i < display.getWidth(); i++) {
    for (int j = 0; j < display.getHeight(); j++) {
      display.setPixel(i, j, CHSV((hue + i + j) % 256, 255, 255));
    }
  }
  hue++;
  FastLED.show();
  Serial.print("time:");
  Serial.println(millis() - time);
  // TEST SEGMENT*/

  static short bg = preferences.getShort("bg", 1);
  bg_button.read();
  if (bg_button.isFallingEdge()) {
    bg = (bg + 1) % 4;
  }
  if (bg_button.isRisingEdge()) {
    preferences.putShort("bg", bg);
  }

  static short brightness = preferences.getShort("brightness", 1);
  brightness_button.read();
  if (brightness_button.isFallingEdge()) {
    brightness = !brightness;
    brightness ? FastLED.setBrightness(1) : FastLED.setBrightness(16);
  }
  if (brightness_button.isRisingEdge()) {
    preferences.putShort("brightness", brightness);
  }

  static long lastFrame = 0;
  if (true) {  // millis() - lastFrame > 50) {
    Serial.println(millis() - lastFrame);
    lastFrame = millis();

    switch (bg) {
      case 0:
        matriceRgb(&display, brightness);
        break;
      case 1:
        matrix(&display);
        break;
      case 2:
        fire(&display);
        break;
      case 3:
        epilepsie(&display);
        break;
      default:
        break;
    }
    FastLED.show();
  }
  // delay(10);
}