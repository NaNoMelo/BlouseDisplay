#ifndef DISPLAY_H
#define DISPLAY_H

#include <FastLED.h>

#include "DisplayCTL.h"
#include "DisplayGFX.h"

class Display
{
private:
    int width;
    int height;
    int format;

    DisplayCTL controller;
    /* data */
public:
    Display(int width, int height, int format);
    ~Display();
};

#endif // DISPLAY_H