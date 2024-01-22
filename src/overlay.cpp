#include "overlay.h"

const bool N[] = {
#if FORMAT == 0
    1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1 // 6x8
#else
    1, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 1 // 6x6
#endif
};

const bool a[]{
    0, 1, 1, 1, 0,
    0, 0, 0, 0, 1,
    0, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 1}; // 5x6

const bool o[]{
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0}; // 5x6

void overlayBuilder(CRGB overlay[WIDTH][HEIGHT], const bool pattern[], int width, int height, int xOffset, int yOffset)
{
    for (int x = xOffset; x < xOffset + width; x++)
    {
        if (x < WIDTH && x >= 0)
        {
            for (int y = yOffset; y < yOffset + height; y++)
            {
                if (y < HEIGHT && y >= 0)
                {
                    if (pattern[(y - yOffset) * width + (x - xOffset)])
                    {
                        overlay[x][y] = CRGB::White;
                    }
                    else
                    {
                        overlay[x][y] = CRGB::Black;
                    }
                }
            }
        }
    }
}

void NaNoverlay(CRGB overlay[WIDTH][HEIGHT], int originX, int originY)
{
#if FORMAT == 1
    overlayBuilder(overlay, N, 6, 6, originX, originY);
    overlayBuilder(overlay, a, 5, 6, originX + 7, originY);
    overlayBuilder(overlay, N, 6, 6, originX + 13, originY);
    overlayBuilder(overlay, o, 5, 6, originX + 20, originY);
#else
    overlayBuilder(overlay, N, 6, 8, originX, originY);
    overlayBuilder(overlay, a, 5, 6, originX + 7, originY + 2);
    overlayBuilder(overlay, N, 6, 8, originX + 13, originY);
    overlayBuilder(overlay, o, 5, 6, originX + 20, originY + 2);
#endif
}

#if FORMAT == 1
void nanoScroll()
{
    static int overlayState = 0;
    if (overlayState <= 0)
        overlayState = 35;
    NaNoverlay(overlayState, 1);
    NaNoverlay(overlayState - 35, 1);
    overlayState--;
}
#endif