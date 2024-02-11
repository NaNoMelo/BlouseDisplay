#ifndef DISPLAYCTL_H
#define DISPLAYCTL_H

#include <Arduino.h>
#include <FastLED.h>

class DisplayCTL
{
private:
    int width;
    int height;
    int format;

    CRGB top[256];
    CRGB bottom[256];
    CRGB leds[128];
    /* data */
public:
    DisplayCTL(int width, int height, int format);
    ~DisplayCTL();
};

#endif // DISPLAYCTL_H