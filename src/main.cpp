#include <Arduino.h>
#include <Preferences.h>
#include <stdlib.h>
#include <time.h>

#include "background.h"
#include "overlay.h"

Preferences preferences;

#define FORMAT 0

#define BG_BUTTON_PIN 19
#define MODE_BUTTON_PIN 18

#if FORMAT == 0
#define WIDTH 32
#define HEIGHT 16
#define BOT_LED_PIN 25
#define TOP_LED_PIN 27
CRGB top[256];
CRGB bot[256];
#endif

#if FORMAT == 1
#define WIDTH 16
#define HEIGHT 8
#define LED_PIN 2
CRGB leds[128];
#endif

void matriceToLed();
void buildMatrice();

CRGB matrice[WIDTH][HEIGHT];
CRGB background[WIDTH][HEIGHT];
CRGB overlay[WIDTH][HEIGHT];

void setup() {
  Serial.begin(115200);
  preferences.begin("display");
#if FORMAT == 0
  FastLED.addLeds<NEOPIXEL, TOP_LED_PIN>(top, 8 * 32);
  FastLED.addLeds<NEOPIXEL, BOT_LED_PIN>(bot, 8 * 32);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
#endif
#if FORMAT == 1
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 8 * 16);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
#endif
  FastLED.setMaxRefreshRate(30);
  FastLED.setBrightness(1);
  srand(time(NULL));
  pinMode(BG_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  NaNoverlay(overlay, 3, 4);
}

void loop() {
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
        matriceRgb(background, brightness);
        break;
      case 2:
        matrix(background);
        break;
      case 3:
        fire(background);
        break;
      case 4:
        epilepsie(background);
        break;
      default:
        break;
    }
  }

#if FORMAT == 1
  if (!(counter % 10)) {
    for (int x = 0; x < WIDTH; x++) {
      for (int y = 0; y < HEIGHT; y++) {
        overlay[x][y] = CRGB::Black;
      }
    }
    nanoScroll(overlay);
  }
#endif

  if (!(counter % 5)) {
    buildMatrice();
    matriceToLed();
    FastLED.show();
  }
  delay(10);
  counter++;
}

void buildMatrice() {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      if (overlay[x][y] == CRGB::Black) {
        matrice[x][y] = background[x][y];
      } else {
        matrice[x][y] = overlay[x][y];
      }
    }
  }
}

void matriceToLed() {
  CRGB color;
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      color = matrice[x][y];
#if FORMAT == 0
      if (y >= 8) {
        if (x % 2) {
          bot[x * 8 - y + 15] = color;
        } else {
          bot[x * 8 + y - 8] = color;
        }
      } else {
        if (x % 2) {
          top[x * 8 - y + 7] = color;
        } else {
          top[x * 8 + y] = color;
        }
      }
#endif
#if FORMAT == 1
      if (x % 2) {
        leds[x * 8 - y + 7] = matrice[x][y];
      } else {
        leds[x * 8 + y] = matrice[x][y];
      }
#endif
    }
  }
}