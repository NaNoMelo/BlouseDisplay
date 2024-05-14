#ifndef DISPLAYASSEMBLY_H
#define DISPLAYASSEMBLY_H

#include <Arduino.h>
#include <FastLED.h>

#include "DisplayCTL.hpp"

enum DARotation { R_0 = 0, R_90 = 1, R_180 = 2, R_270 = 3 };
struct DCTLList {
  DisplayCTL *controller;
  int x;
  int y;
  DARotation rotation = R_0;
  DCTLList *next = NULL;

  DCTLList(int width, int height, uint8_t pin, DCTLFormat format = VERTICAL,
           int xPos = 0, int yPos = 0, DARotation rotation = R_0) {
    controller = new DisplayCTL(width, height, pin, format);
    x = xPos;
    y = yPos;
    this->rotation = rotation;
  }

  DCTLList(DisplayCTL *controller, int xPos = 0, int yPos = 0,
           DARotation rotation = R_0) {
    this->controller = controller;
    x = xPos;
    y = yPos;
    this->rotation = rotation;
  }
};

class DisplayAssembly {
 private:
  DCTLList *controllers = NULL;
  int nbControllers = 0;
  int aWidth = 0;
  int aHeight = 0;
  CRGB ***matrice = NULL;

 public:
  void addController(DisplayCTL *controller, int xPos = 0, int yPos = 0,
                     DARotation rotation = R_0);
  void addController(int width, int height, uint8_t pin,
                     DCTLFormat format = VERTICAL, int xPos = 0, int yPos = 0,
                     DARotation rotation = R_0);
  void updateMatrice();
  int getWidth(){ return aWidth; }
  int getHeight(){ return aHeight; }
  CRGB ***getMatrice(){ return matrice; }
  ~DisplayAssembly();
};

#endif  // DISPLAYASSEMBLY_H