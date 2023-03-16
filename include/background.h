#ifndef BLOUSEDISPLAY_BACKGROUND_H
#define BLOUSEDISPLAY_BACKGROUND_H

#include <Arduino.h>
#include <FastLED.h>
#include <stdlib.h>

#define FORMAT 0
#if FORMAT == 0
#define WIDTH 32
#define HEIGHT 16
#endif
#if FORMAT == 1
#define WIDTH 16
#define HEIGHT 8
#endif

void matriceRgb(CRGB background[WIDTH][HEIGHT]);
void matrix(CRGB background[WIDTH][HEIGHT]);
void fire(CRGB background[WIDTH][HEIGHT]);

#endif // BLOUSEDISPLAY_BACKGROUND_H