#ifndef BLOUSEDISPLAY_OVERLAY_H
#define BLOUSEDISPLAY_OVERLAY_H

#include "DisplayAssembly.hpp"

void NaNoverlay(DisplayAssembly *display, int originX, int originY);
void overlayBuilder(DisplayAssembly *display, bool pattern[], int width,
                    int height, int xOffset, int yOffset);

#if FORMAT == 1
void nanoScroll(DisplayAssembly *display);
#endif

#endif  // BLOUSEDISPLAY_OVERLAY_H