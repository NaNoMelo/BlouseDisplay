#ifndef DISPLAY_H
#define DISPLAY_H

#include <FastLED.h>

#include "DisplayAssembly.h"

class Display {
 private:
  DisplayAssembly* controllers;

 public:
  Display();
  ~Display();
};

#endif  // DISPLAY_H