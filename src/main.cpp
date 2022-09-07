#include <Arduino.h>
#include <FastLED.h>
#include <stdlib.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <string.h>

#define FORMAT 0

#define BUTTON_PIN 19

#if FORMAT == 0
#define WIDTH 32
#define HEIGHT 16
#define BOT_LED_PIN 25
#define TOP_LED_PIN 27
CRGB top[256];
CRGB bot[256];
#endif

#if FORMAT == 1
#define WIDTH 16
#define HEIGHT 8
#define LED_PIN 12
CRGB leds[128];
#endif

void matriceToLed();
void rgbNext(int nb);
void matriceRgb();
void matrix();
void fire();
void NaNoverlay();
void overlayBuilder(bool pattern[], int width, int height, int xOffset, int yOffset);
void buildMatrice();

bool N[] = {
    1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1}; // 6x8

bool a[]{
    0, 1, 1, 1, 0,
    0, 0, 0, 0, 1,
    0, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 1}; // 5x6

bool o[]{
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0}; // 5x6

unsigned char matrice[WIDTH][HEIGHT][3];
unsigned char background[WIDTH][HEIGHT][3];
unsigned char overlay[WIDTH][HEIGHT][3];

/*bool deviceConnected = false;

BLECharacteristic presetCharacteristics(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
BLEDescriptor presetDescriptor(BLEUUID((uint16_t)0x2902));
class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};*/

void setup()
{
#if FORMAT == 0
  FastLED.addLeds<NEOPIXEL, TOP_LED_PIN>(top, 8 * 32);
  FastLED.addLeds<NEOPIXEL, BOT_LED_PIN>(bot, 8 * 32);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);
#endif
#if FORMAT == 1
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 8 * 16);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
#endif
  FastLED.setMaxRefreshRate(0);
  srand(time(NULL));
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.setBrightness(8);
  NaNoverlay();
}

int r = 255, g = 0, b = 0;
int pr, pg, pb;
int mode = 2;
int timer = 1000;
int hue = 0;
void loop()
{
  if (!digitalRead(BUTTON_PIN))
  {
    mode++;
    if (mode > 3)
      mode = 1;
    while (!digitalRead(BUTTON_PIN))
      delay(100);
  }
  switch (mode)
  {
  case 1:
    pr = r, pg = g, pb = b;
    matriceRgb();
    r = pr, g = pg, b = pb;
    rgbNext(4);
    timer = 10;
    break;
  case 2:
    matrix();
    timer = 50;
    break;
  case 3:
    fire();
    timer = 50;
    break;
  default:
    break;
  }
  buildMatrice();
  matriceToLed();
  FastLED.show();
  delay(timer);
}

void matriceRgb()
{
  int ir, ig, ib;
  for (int i = 0; i < WIDTH + HEIGHT; i++)
  {
    for (int x = 0; x < WIDTH; x++)
    {
      for (int y = 0; y < HEIGHT; y++)
      {
        if (x + y == i)
        {
          background[x][y][0] = r / 2, background[x][y][1] = g / 2, background[x][y][2] = b / 2;
        }
      }
    }
    rgbNext(8);
  }
}

int alea;
void matrix()
{
  for (int y = HEIGHT - 1; y > 0; y--)
  {
    for (int x = 0; x < WIDTH; x++)
    {
      background[x][y][0] = background[x][y - 1][0];
      background[x][y][1] = background[x][y - 1][1];
      background[x][y][2] = background[x][y - 1][2];
    }
  }
  for (int x = 0; x < WIDTH; x++)
  {
    background[x][0][0] = 0;
    background[x][0][1] = 0;
    background[x][0][2] = 0;
  }
  for (int i = 0; i < (FORMAT ? 1 : 3); i++)
  {
    alea = rand() % WIDTH;
    background[alea][0][0] = 0;
    background[alea][0][1] = 255;
    background[alea][0][2] = 0;
  }
}

int randFeu[WIDTH];
float etatFeu[WIDTH];
void fire()
{
  for (int i = 0; i < WIDTH; i++)
  {
    randFeu[i] = rand() % HEIGHT;
    switch (i)
    {
    case 0:
      etatFeu[i] = (etatFeu[i] + randFeu[i] + randFeu[i + 1]) / 3;
      break;

    case WIDTH - 1:
      etatFeu[i] = (etatFeu[i] + randFeu[i - 1] + randFeu[i]) / 3;
      break;

    default:
      etatFeu[i] = (etatFeu[i] + randFeu[i - 1] + randFeu[i] + randFeu[i + 1]) / 4;
      break;
    }
  };
  CRGB color;
  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y < HEIGHT; y++)
    {
      if (y < etatFeu[x])
      {
        background[x][y][0] = 0;
        background[x][y][1] = 0;
        background[x][y][2] = 0;
      }
      else
      {
        color = CHSV(map(constrain(y - etatFeu[x], 0, HEIGHT - 1), 0, HEIGHT - 1, 0, 60), 255, 255);
        background[x][y][0] = color.r;
        background[x][y][1] = color.g;
        background[x][y][2] = color.b;
      }
    }
  }
}

void matriceToLed()
{
#if FORMAT == 0
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
          bot[x * 8 + y - 8].g = matrice[x][y][1];
          bot[x * 8 + y - 8].b = matrice[x][y][2];
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
          top[x * 8 + y].g = matrice[x][y][1];
          top[x * 8 + y].b = matrice[x][y][2];
        }
      }
    }
  }
#endif
#if FORMAT == 1
  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y < HEIGHT; y++)
    {
      if (x % 2)
      {
        leds[x * 8 - y + 7].r = matrice[x][y][0];
        leds[x * 8 - y + 7].g = matrice[x][y][1];
        leds[x * 8 - y + 7].b = matrice[x][y][2];
      }
      else
      {
        leds[x * 8 + y].r = matrice[x][y][0];
        leds[x * 8 + y].g = matrice[x][y][1];
        leds[x * 8 + y].b = matrice[x][y][2];
      }
    }
  }
#endif
}

void overlayBuilder(bool pattern[], int width, int height, int xOffset, int yOffset)
{
  for (int x = xOffset; x < xOffset + width; x++)
  {
    if (x < WIDTH)
    {
      for (int y = yOffset; y < yOffset + height; y++)
      {
        if (y < HEIGHT)
        {
          if (pattern[(y - yOffset) * width + (x - xOffset)])
          {
            overlay[x][y][0] = 255;
            overlay[x][y][1] = 255;
            overlay[x][y][2] = 255;
          }
          else
          {
            overlay[x][y][0] = 0;
            overlay[x][y][1] = 0;
            overlay[x][y][2] = 0;
          }
        }
      }
    }
  }
}

#if FORMAT == 1
int overlayState = 0;
#endif
void NaNoverlay()
{
#if FORMAT == 0
  int originX = 3;
  int originY = 4;
  overlayBuilder(N, 6, 8, originX, originY);
  overlayBuilder(a, 5, 6, originX + 7, originY + 2);
  overlayBuilder(N, 6, 8, originX + 13, originY);
  overlayBuilder(o, 5, 6, originX + 20, originY + 2);
#endif
#if FORMAT == 1
  if (overlayState >= 25)
    overlayState = 0;
  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y < HEIGHT; y++)
    {
    }
  }
  overlayState++;
#endif
}

void buildMatrice()
{
  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y < HEIGHT; y++)
    {
      if (!overlay[x][y][0] && !overlay[x][y][1] && !overlay[x][y][2])
      {
        matrice[x][y][0] = background[x][y][0];
        matrice[x][y][1] = background[x][y][1];
        matrice[x][y][2] = background[x][y][2];
      }
      else
      {
        matrice[x][y][0] = overlay[x][y][0];
        matrice[x][y][1] = overlay[x][y][1];
        matrice[x][y][2] = overlay[x][y][2];
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