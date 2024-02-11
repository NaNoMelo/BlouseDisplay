#include "display.h"


Display::Display(int width, int height, int format)
{
  this->width = width;
  this->height = height;
  this->format = format;

  this->background = calloc(sizeof(CRGB *), height);
  this->overlay = malloc(sizeof(CRGB) * width * height);
}

/**
 * TODO
 * fonctionnement des animations par sections en liste chainée
 * - plusieurs listes associées chacune à une animation
 * - les pixels sont assignés à une section et a un index dans la section en elle même
 * - chaque frame, chaque index prends l'état du précédent de la section, et le premier de la section prends un état défini par l'animation
 *
 */



matriceToLed()
{
  CRGB color;
  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y < HEIGHT; y++)
    {
      color = matrice[x][y];
#if FORMAT == 0
      if (y >= 8)
      {
        if (x % 2)
        {
          bot[x * 8 - y + 15] = color;
        }
        else
        {
          bot[x * 8 + y - 8] = color;
        }
      }
      else
      {
        if (x % 2)
        {
          top[x * 8 - y + 7] = color;
        }
        else
        {
          top[x * 8 + y] = color;
        }
      }
#endif
#if FORMAT == 1
      if (x % 2)
      {
        leds[x * 8 - y + 7] = matrice[x][y];
      }
      else
      {
        leds[x * 8 + y] = matrice[x][y];
      }
#endif
    }
  }
}
