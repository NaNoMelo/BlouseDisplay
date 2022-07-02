#include <Arduino.h>
#include <FastLED.h>
#include <BluetoothSerial.h>

#define WIDTH 32
#define HEIGHT 16
#define BOT_LED_PIN 23
#define TOP_LED_PIN 22

void matriceToLed();
void rgbNext(int nb);
void matriceRgb();
void NaNoverlay();

int NaNo[8][25] = {
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0}};

int a[6][5] = {
    {0, 1, 1, 1, 0},
    {0, 0, 0, 0, 1},
    {0, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 1}};

int o[5][5] = {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}};

CRGB top[256];
CRGB bot[256];

int matrice[WIDTH][HEIGHT][3];
void setup()
{
  FastLED.addLeds<NEOPIXEL, TOP_LED_PIN>(top, 8 * 32);
  FastLED.addLeds<NEOPIXEL, BOT_LED_PIN>(bot, 8 * 32);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  FastLED.setBrightness(64);
}

int r = 255, g = 0, b = 0;
int pr, pg, pb;
void loop()
{

  pr = r, pg = g, pb = b;
  matriceRgb();
  NaNoverlay();
  matriceToLed();
  FastLED.show();
  r = pr, g = pg, b = pb;
  rgbNext(32);
  delay(50);
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
          matrice[x][y][0] = r / 4, matrice[x][y][1] = g / 4, matrice[x][y][2] = b / 4;
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
          bot[x * 8 - y + 15].r = matrice[x][y][0];
          bot[x * 8 - y + 15].g = matrice[x][y][1];
          bot[x * 8 - y + 15].b = matrice[x][y][2];
        }
        else
        {
          bot[x * 8 + y - 8].r = matrice[x][y][0];
          bot[x * 8 + y - 8].b = matrice[x][y][2];
          bot[x * 8 + y - 8].g = matrice[x][y][1];
        }
      }
      else
      {
        if (x % 2)
        {
          top[x * 8 - y + 7].r = matrice[x][y][0];
          top[x * 8 - y + 7].g = matrice[x][y][1];
          top[x * 8 - y + 7].b = matrice[x][y][2];
        }
        else
        {
          top[x * 8 + y].r = matrice[x][y][0];
          top[x * 8 + y].b = matrice[x][y][2];
          top[x * 8 + y].g = matrice[x][y][1];
        }
        // if (x % 2)
        // {
        //   top[255 - x * 8 - y].r = matrice[x][y][0];
        //   top[255 - x * 8 - y].g = matrice[x][y][1];
        //   top[255 - x * 8 - y].b = matrice[x][y][2];
        // }
        // else
        // {
        //   top[248 - x * 8 + y].r = matrice[x][y][0];
        //   top[248 - x * 8 + y].g = matrice[x][y][1];
        //   top[248 - x * 8 + y].b = matrice[x][y][2];
        // }
      }
    }
  }
}

void NaNoverlay()
{
  for (int x = 0; x < 25; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      if (NaNo[y][x] == 1)
      {
        matrice[3 + x][4 + y][0] = 255;
        matrice[3 + x][4 + y][1] = 255;
        matrice[3 + x][4 + y][2] = 255;
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