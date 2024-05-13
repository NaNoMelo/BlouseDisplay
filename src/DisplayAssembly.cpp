#include "DisplayAssembly.h"

#include "Display.h"

/**
 * @brief Get the updated x coord of a controller after a clockwise rotation
 * @param r Rotation value
 * @param x x coord within the controller
 * @param y y coord within the controller
 * @return rotated x coord
 */
#define R_X(r, x, y) (r % 2 ? y : x) * (1 - 2 * (1 & r == 2 & r))
/**
 * @brief Get the updated y coord of a controller after a clockwise rotation
 * @param r Rotation value
 * @param x x coord within the controller
 * @param y y coord within the controller
 * @return rotated y coord
 */
#define R_Y(r, x, y) (r % 2 ? x : y) * (1 - 2 * (2 & r))
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

void DisplayAssembly::addController(DisplayCTL *controller, int xPos = 0,
                                    int yPos = 0, DARotation rotation = R_0) {
  DCTLList *new_ctl = new DCTLList(controller, xPos, yPos, rotation);
  new_ctl->next = controllers;
  controllers = new_ctl;
  nbControllers++;
}

void DisplayAssembly::addController(int width, int height, uint8_t pin,
                                    DCTLFormat format = VERTICAL, int xPos = 0,
                                    int yPos = 0, DARotation rotation = R_0) {
  DisplayCTL *new_ctl = new DisplayCTL(width, height, pin, format);
  addController(new_ctl, xPos, yPos, rotation);
}

void DisplayAssembly::updateMatrice() {
  DCTLList *current = controllers;
  while (current != NULL) {
    DARotation rotation = current->rotation;
    int width = current->controller->getWidth();
    int height = current->controller->getHeight();
    aWidth = max(aWidth, current->x + R_X(rotation, width, height));
    aHeight = max(aHeight, current->y + R_Y(rotation, width, height));
    current = current->next;
  }
  if (matrice != NULL) {
    delete[] matrice;
    delete matrice;
  }
  matrice = new CRGB **[aWidth];
  for (int i = 0; i < aWidth; i++) {
    matrice[i] = new CRGB *[aHeight];
  }

  current = controllers;
  int usedPins[24] = {0};
  DisplayCTL *controller;
  while (current != NULL) {
    // Creation of the link beetwen the controller and the matrice
    controller = current->controller;
    DARotation rotation = current->rotation;
    int width = controller->getWidth();
    int height = controller->getHeight();
    int params[] = {width, -height, -width, +height};
    int rotWidth = params[rotation % 4];
    int rotHeight = params[rotation - 1 % 4];
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        int x = current->x + R_X(rotation, i, j);
        int y = current->y + R_Y(rotation, i, j);
        matrice[x][y] =
            const_cast<CRGB *>(&controller->leds[controller->getIndex(i, j)]);
      }
    }

    // Set up the controller with the right offset
    controller->offset +=
        controller->width * controller->height * usedPins[controller->pin];
    usedPins[controller->pin]++;
    controller->setupLeds();
    current = current->next;
  }

  for (int i = 0; i < aWidth; i++) {
    for (int j = 0; j < aHeight; j++) {
      if (matrice[i][j] == NULL) {
        matrice[i][j] = const_cast<CRGB *>(new CRGB(0, 0, 0));
      }
    }
  }
}

DisplayAssembly::~DisplayAssembly() {
  DCTLList *current = controllers;
  while (current != NULL) {
    DCTLList *next = current->next;
    delete current;
    current = next;
  }
  if (matrice != NULL) {
    delete[] matrice;
    delete matrice;
  }
}