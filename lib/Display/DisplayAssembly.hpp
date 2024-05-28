#ifndef DISPLAYASSEMBLY_H
#define DISPLAYASSEMBLY_H

#include <Arduino.h>
#include <FastLED.h>

#include "DisplayCTL.hpp"

/**
 * @brief Get the updated x coord of a controller after a clockwise rotation
 * @param r Rotation value
 * @param x x coord within the controller
 * @param y y coord within the controller
 * @return rotated x coord
 */
#define R_X(r, x, y) (r % 2 ? y : x) * (2 * ((1 & r) == ((2 & r) >> 1)) - 1)
/**
 * @brief Get the updated y coord of a controller after a clockwise rotation
 * @param r Rotation value
 * @param x x coord within the controller
 * @param y y coord within the controller
 * @return rotated y coord
 */
#define R_Y(r, x, y) (r % 2 ? x : y) * (1 - (2 & r))
/**
 * rotation 0: (value 0)
 * x = x
 * y = y
 * rotation 90: (value 1)
 * x = -y
 * y = x
 * rotation 180: (value 2)
 * x = -x
 * y = -y
 * rotation 270: (value 3)
 * x = y
 * y = -x
 */

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
struct CRGBList {
  CRGB *pixel;
  CRGBList *next = NULL;

  CRGBList(CRGB *pixel, CRGBList *next = NULL) {
    this->pixel = pixel;
    this->next = next;
  }
};

class DisplayAssembly {
 private:
  DCTLList *controllers = NULL;
  int nbControllers = 0;
  int aWidth = 0;
  int aHeight = 0;
  CRGB ***matrice = NULL;
  CRGBList *voidPixels = NULL;

 public:
  void addController(DisplayCTL *controller, int xPos = 0, int yPos = 0,
                     DARotation rotation = R_0);
  void addController(int width, int height, uint8_t pin,
                     DCTLFormat format = VERTICAL, int xPos = 0, int yPos = 0,
                     DARotation rotation = R_0);
  void updateMatrice();
  int getWidth() { return aWidth; }
  int getHeight() { return aHeight; }
  CRGB ***getMatrice() { return matrice; }
  ~DisplayAssembly();
};

#endif  // DISPLAYASSEMBLY_H