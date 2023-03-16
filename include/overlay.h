#ifndef BLOUSEDISPLAY_OVERLAY_H
#define BLOUSEDISPLAY_OVERLAY_H

#include <Arduino.h>
#include <FastLED.h>
#include <stdlib.h>

#if FORMAT == 0
#define WIDTH 32
#define HEIGHT 16
#endif
#if FORMAT == 1
#define WIDTH 16
#define HEIGHT 8
#endif

void NaNoverlay(CRGB overlay[WIDTH][HEIGHT], int originX, int originY);
void overlayBuilder(CRGB overlay[WIDTH][HEIGHT], bool pattern[], int width, int height, int xOffset, int yOffset);
void nanoScroll(CRGB overlay[WIDTH][HEIGHT]);

#endif // BLOUSEDISPLAY_OVERLAY_H