#ifndef BLOUSEDISPLAY_BACKGROUND_H
#define BLOUSEDISPLAY_BACKGROUND_H

#include <Arduino.h>
#include <FastLED.h>
#include <stdlib.h>

#include "DisplayAssembly.hpp"

void matriceRgb(DisplayAssembly display, int mode);
void matrix(DisplayAssembly display);
void fire(DisplayAssembly display);
void epilepsie(DisplayAssembly display);

#endif // BLOUSEDISPLAY_BACKGROUND_H