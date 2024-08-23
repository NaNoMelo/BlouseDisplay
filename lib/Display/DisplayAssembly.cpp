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
  if (_controllers != NULL) {
    delete _controllers;
  }
}

void DisplayAssembly::setPixel(int x, int y, CRGB color) {
  if (x < _min_x || x > _max_x || y < _min_y || y > _max_y) return;
  DCTLList *current = _controllers;
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

  _min_x = min(_min_x, new_ctl->min_x);
  _min_y = min(_min_y, new_ctl->min_y);
  _max_x = max(_max_x, new_ctl->max_x);
  _max_y = max(_max_y, new_ctl->max_y);
  _width = _max_x - _min_x;
  _height = _max_y - _min_y;
  _nbControllers++;

  new_ctl->next = _controllers;
  _controllers = new_ctl;
}

void DisplayAssembly::addController(int width, int height, uint8_t pin,
                                    DCTLFormat format, int xPos, int yPos,
                                    DARotation rotation) {
  DisplayCTL *new_ctl = new DisplayCTL(width, height, pin, format);
  addController(new_ctl, xPos, yPos, rotation, false);
}