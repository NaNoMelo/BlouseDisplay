#include "overlay.h"

const bool N[] = {
    1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1,
    0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1  // 6x8
};

const bool a[]{0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1,
               1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1};  // 5x6

const bool o[]{0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
               1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0};  // 5x6

void overlayBuilder(DisplayAssembly *display, const bool pattern[], int width,
                    int height, int xOffset, int yOffset) {
  for (int x = xOffset; x < xOffset + width; x++) {
    if (x < display->getWidth() && x >= 0) {
      for (int y = yOffset; y < yOffset + height; y++) {
        if (y < display->getHeight() && y >= 0) {
          if (pattern[(y - yOffset) * width + (x - xOffset)]) {
            display->setPixel(x, y, CRGB::White);
          }
        }
      }
    }
  }
}

void NaNoverlay(DisplayAssembly *display, int originX, int originY) {
  overlayBuilder(display, N, 6, 8, originX, originY);
  overlayBuilder(display, a, 5, 6, originX + 7, originY + 2);
  overlayBuilder(display, N, 6, 8, originX + 13, originY);
  overlayBuilder(display, o, 5, 6, originX + 20, originY + 2);
}

#if FORMAT == 1
void nanoScroll() {
  static int overlayState = 0;
  if (overlayState <= 0) overlayState = 35;
  NaNoverlay(overlayState, 1);
  NaNoverlay(overlayState - 35, 1);
  overlayState--;
}
#endif