#include <Arduino.h>
#include <FastLED.h>

#define WIDTH 32
#define HEIGHT 16
#define DATA_PIN 23

void matriceToLed();
void rgbNext(int nb);
void matriceRgb();

int vide[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

int N[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 0, 0, 0, 1, 0},
    {0, 1, 0, 1, 0, 0, 1, 0},
    {0, 1, 0, 0, 1, 0, 1, 0},
    {0, 1, 0, 0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

int a[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

int o[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

CRGB leds[WIDTH * HEIGHT];

int r = 255, g = 0, b = 0;
int matrice[WIDTH][HEIGHT][3];
int pr, pg, pb;
void setup()
{
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, WIDTH * HEIGHT);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
}

void loop()
{
  FastLED.setBrightness(127 /*map(analogRead(15), 0, 4095, 0, 255)*/);
  pr = r, pg = g, pb = b;
  matriceRgb();
  matriceToLed();
  FastLED.show();
  r = pr, g = pg, b = pb;
  rgbNext(32);
  delay(100);
}

void matriceRgb()
{
  int ir, ig, ib;
  for (int i = 0; i < WIDTH + HEIGHT; i++)
  {
    ir = r, ig = g, ib = b;
    for (int x = 0; x < WIDTH; x++)
    {
      for (int y = 0; y < HEIGHT; y++)
      {
        if (x + y == i)
        {
          matrice[x][y][0] = r, matrice[x][y][1] = g, matrice[x][y][2] = b;
        }
      }
    }
    rgbNext(32);
  }
}

void matriceToLed()
{
  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y < HEIGHT; y++)
    {
      if (y >= 8)
      {
        if (x % 2)
        {
          leds[x * 8 - y + 15].r = matrice[x][y][0];
          leds[x * 8 - y + 15].g = matrice[x][y][1];
          leds[x * 8 - y + 15].b = matrice[x][y][2];
        }
        else
        {
          leds[x * 8 + y - 8].r = matrice[x][y][0];
          leds[x * 8 + y - 8].g = matrice[x][y][1];
          leds[x * 8 + y - 8].b = matrice[x][y][2];
        }
      }
      else
      {
        if (x % 2)
        {
          leds[511 - x * 8 - y].r = matrice[x][y][0];
          leds[511 - x * 8 - y].g = matrice[x][y][1];
          leds[511 - x * 8 - y].b = matrice[x][y][2];
        }
        else
        {
          leds[504 - x * 8 + y].r = matrice[x][y][0];
          leds[504 - x * 8 + y].g = matrice[x][y][1];
          leds[504 - x * 8 + y].b = matrice[x][y][2];
        }
      }
    }
  }
}

void rgbNext(int nb)
{
  for (int i = 0; i < nb; i++)
  {
    if (r > 0 && b == 0)
    {
      r--;
      g++;
    }
    if (g > 0 && r == 0)
    {
      g--;
      b++;
    }
    if (b > 0 && g == 0)
    {
      r++;
      b--;
    }
  }
}