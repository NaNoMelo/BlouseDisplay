#include "DisplayCTL.h"

DisplayCTL::DisplayCTL(int width, int height, int format)
{
    this->width = width;
    this->height = height;
    this->format = format;

    FastLED.addLeds<WS2812B, 2, GRB>(this->top, 256);
    FastLED.addLeds<WS2812B, 3, GRB>(this->bottom, 256);
    FastLED.addLeds<WS2812B, 4, GRB>(this->leds, 128);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
    FastLED.setMaxRefreshRate(30);
    FastLED.setBrightness(1);
    
}


int index(int x, int y)
{
}