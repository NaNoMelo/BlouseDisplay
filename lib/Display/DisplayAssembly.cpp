#include "DisplayAssembly.hpp"

// DCTLList

DCTLList::DCTLList(DisplayCTL *controller, int xPos, int yPos,
                   DARotation rotation, bool external) {
  this->controller = controller;
  x = xPos;
  y = yPos;
  this->rotation = rotation;

  int width = controller->getWidth();
  int height = controller->getHeight();
  min_x = min(x, x + R_X(rotation, width, height));
  min_y = min(y, y + R_Y(rotation, width, height));
  max_x = max(x, x + R_X(rotation, width, height));
  max_y = max(y, y + R_Y(rotation, width, height));

  this->external = external;
}

DCTLList::~DCTLList() {
  if (!external) delete controller;
  if (next != NULL) {
    delete next;
  }
}

// DisplayAssembly

DisplayAssembly::~DisplayAssembly() {
  if (controllers != NULL) {
    delete controllers;
  }
}

void DisplayAssembly::setPixel(int x, int y, CRGB color) {
  if (x < min_x || x > max_x || y < min_y || y > max_y) return;
  DCTLList *current = controllers;
  while (current != NULL) {
    if (x >= current->min_x && x <= current->max_x && y >= current->min_y &&
        y <= current->max_y) {
      int inverseRotation = (4 - current->rotation) % 4;
      int cX = R_X(inverseRotation, x - current->min_x, y - current->min_y);
      int cY = R_Y(inverseRotation, x - current->min_x, y - current->min_y);
      current->controller->setPixel(cX, cY, color);
      return;
    }
  }
}

void DisplayAssembly::addController(DisplayCTL *controller, int xPos, int yPos,
                                    DARotation rotation, bool external) {
  DCTLList *new_ctl = new DCTLList(controller, xPos, yPos, rotation, external);

  min_x = min(min_x, new_ctl->min_x);
  min_y = min(min_y, new_ctl->min_y);
  max_x = max(max_x, new_ctl->max_x);
  max_y = max(max_y, new_ctl->max_y);
  width = max_x - min_x;
  height = max_y - min_y;
  nbControllers++;

  new_ctl->next = controllers;
  controllers = new_ctl;
}

void DisplayAssembly::addController(int width, int height, uint8_t pin,
                                    DCTLFormat format, int xPos, int yPos,
                                    DARotation rotation) {
  DisplayCTL *new_ctl = new DisplayCTL(width, height, pin, format);
  addController(new_ctl, xPos, yPos, rotation, false);
}