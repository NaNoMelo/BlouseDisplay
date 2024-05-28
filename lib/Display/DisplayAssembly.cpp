#include "DisplayAssembly.hpp"

void DisplayAssembly::addController(DisplayCTL *controller, int xPos, int yPos,
                                    DARotation rotation) {
  DCTLList *new_ctl = new DCTLList{controller, xPos, yPos, rotation};
  new_ctl->next = controllers;
  controllers = new_ctl;
  nbControllers++;
}

void DisplayAssembly::addController(int width, int height, uint8_t pin,
                                    DCTLFormat format, int xPos, int yPos,
                                    DARotation rotation) {
  DisplayCTL *new_ctl = new DisplayCTL{width, height, pin, format};
  addController(new_ctl, xPos, yPos, rotation);
}

// TODO check the coordinates of the controllers within the matrice
void DisplayAssembly::updateMatrice() {
  DCTLList *current = controllers;
  while (current != NULL) {
    DARotation rotation = current->rotation;
    int width = current->controller->getWidth();
    int height = current->controller->getHeight();
    aWidth = max(
        aWidth, max(current->x + R_X(rotation, width, height), current->x + 1));
    aHeight = max(aHeight, max(current->y + R_Y(rotation, width, height),
                               current->y + 1));
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
  DisplayCTL *controller;
  while (current != NULL) {
    // Creation of the link beetwen the controller and the matrice
    controller = current->controller;
    DARotation rotation = current->rotation;
    int width = controller->getWidth();
    int height = controller->getHeight();
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        int x = current->x + R_X(rotation, i, j);
        int y = current->y + R_Y(rotation, i, j);
        if (x >= 0 && x < aWidth && y >= 0 && y < aHeight)
          matrice[x][y] = &controller->leds[controller->getIndex(i, j)];
      }
    }

    // TODO Set up the controller with the right offset
    // TODO multiple controllers on the same pin
    current = current->next;
  }

  for (int i = 0; i < aWidth; i++) {
    for (int j = 0; j < aHeight; j++) {
      if (matrice[i][j] == NULL) {
        CRGB *pixel = new CRGB{0, 0, 0};
        voidPixels = new CRGBList{pixel, voidPixels};
        matrice[i][j] = pixel;
      }
    }
  }
}

DisplayAssembly::~DisplayAssembly() {
  DCTLList *current = controllers;
  DCTLList *next;
  while (current != NULL) {
    next = current->next;
    delete current;
    current = next;
  }
  if (matrice != NULL) {
    delete[] matrice;
    delete matrice;
  }
  CRGBList *currentPixel = voidPixels;
  CRGBList *nextPixel;
  while (currentPixel != NULL) {
    nextPixel = currentPixel->next;
    delete currentPixel->pixel;
    delete currentPixel;
    currentPixel = nextPixel;
  }
}