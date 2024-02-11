#ifndef DisplayGFX_H
#define DisplayGFX_H

#include <Arduino.h>
#include <FastLED.h>

class DisplayGFX
{
private:
    LinkedList<Pixel> channels;

    CRGB **background;
    CRGB **overlay;
    /* data */
public:
    DisplayGFX(int width, int height, int format);
    ~DisplayGFX();
};

#endif // DisplayGFX_H