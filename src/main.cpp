#include <Arduino.h>
#include <FastLED.h>

#define WIDTH 8
#define HEIGHT 32
#define DATA_PIN 23

void matriceToLed(CRGB *leds, int matrice[8][8], int step, int r, int g, int b);

int brightness = 50;

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
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 1, 0, 0},
    {0, 0, 1, 1, 0, 1, 0, 0},
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

void setup()
{
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, WIDTH * HEIGHT);
  FastLED.setMaxPowerInMilliWatts(5000);
}
int r = 255, g = 0, b = 0;
void loop()
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
  matriceToLed(leds, N, 0, r, g, b);
  matriceToLed(leds, a, 1, r, g, b);
  matriceToLed(leds, N, 2, r, g, b);
  matriceToLed(leds, o, 3, r, g, b);
  FastLED.show();
  delay(10);
}

void matriceToLed(CRGB *leds, int matrice[8][8], int step, int r, int g, int b)
{
  for (int x = 0; x < 8; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      if (!(y % 2))
      {
        leds[x + 8 * y + 64 * step].r = r * matrice[x][y];
        leds[x + 8 * y + 64 * step].g = g * matrice[x][y];
        leds[x + 8 * y + 64 * step].b = b * matrice[x][y];
      }
      else
      {
        leds[x + 8 * y + 64 * step].r = r * matrice[7 - x][y];
        leds[x + 8 * y + 64 * step].g = g * matrice[7 - x][y];
        leds[x + 8 * y + 64 * step].b = b * matrice[7 - x][y];
      }
    }
  }
}