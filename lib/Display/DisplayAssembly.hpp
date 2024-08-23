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

// DCTLList
struct DCTLList {
  DARotation rotation = R_0;
  int x;
  int y;

  int min_x;
  int min_y;
  int max_x;
  int max_y;

  bool external = true;
  DisplayCTL *controller;
  DCTLList *next = NULL;

  DCTLList(DisplayCTL *controller, int xPos = 0, int yPos = 0,
           DARotation rotation = R_0, bool external = true);

  ~DCTLList();
};

// DisplayAssembly
class DisplayAssembly {
 public:
  DisplayAssembly() = default;
  ~DisplayAssembly();

  DisplayAssembly(const DisplayAssembly &) = delete;
  DisplayAssembly &operator=(const DisplayAssembly &) = delete;

  int getWidth() { return _width; }
  int getHeight() { return _height; }
  CRGB getPixel(int x, int y);

  void setPixel(int x, int y, CRGB color);

  void addController(DisplayCTL *controller, int xPos = 0, int yPos = 0,
                     DARotation rotation = R_0, bool external = true);
  void addController(int width, int height, uint8_t pin,
                     DCTLFormat format = VERTICAL, int xPos = 0, int yPos = 0,
                     DARotation rotation = R_0);

 private:
  int _width = 0;
  int _height = 0;

  int _min_x = 0;
  int _min_y = 0;
  int _max_x = 0;
  int _max_y = 0;

  int _nbControllers = 0;
  DCTLList *_controllers = NULL;
};

#endif  // DISPLAYASSEMBLY_H