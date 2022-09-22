#include <Arduino.h>
#include <FastLED.h>
#include <stdlib.h>

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
void matriceRgb();
void matrix();
void fire();
void NaNoverlay(int originX, int originY);
void overlayBuilder(bool pattern[], int width, int height, int xOffset, int yOffset);
void buildMatrice();
void nanoScroll();

bool N[] = {
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

CRGB matrice[WIDTH][HEIGHT];
CRGB background[WIDTH][HEIGHT];
CRGB overlay[WIDTH][HEIGHT];

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
  FastLED.setBrightness(4);
  NaNoverlay(3, 4);
}

int mode = 2;
int timer = 10;
long int counter;
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

  if (!(counter % timer))
  {
    switch (mode)
    {
    case 1:
      timer = 1;
      matriceRgb();
      break;
    case 2:
      timer = 5;
      matrix();
      break;
    case 3:
      timer = 5;
      fire();
      break;
    default:
      break;
    }
  }

  if (FORMAT && !(counter % 10))
  {
    if (FORMAT)
      for (int x = 0; x < WIDTH; x++)
      {
        for (int y = 0; y < HEIGHT; y++)
        {
          overlay[x][y] = CRGB::Black;
        }
      }
    nanoScroll();
  }
  if (!(counter % timer))
  {
    buildMatrice();
    matriceToLed();
    FastLED.show();
  }
  delay(10);
  counter++;
}

short int hue = 0;
void matriceRgb()
{
  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y < HEIGHT; y++)
    {
      background[x][y].setHSV(map(x + y + hue, 0, 4 * (WIDTH + HEIGHT), 0, 255), 255, 255) /= 2;
    }
  }
  hue += 1;
}

int alea;
void matrix()
{
  for (int y = HEIGHT - 1; y > 0; y--)
  {
    for (int x = 0; x < WIDTH; x++)
    {
      background[x][y] = background[x][y - 1];
    }
  }
  for (int x = 0; x < WIDTH; x++)
  {
    background[x][0] = CRGB::Black;
  }
  for (int i = 0; i < (FORMAT ? 1 : 3); i++)
  {
    alea = rand() % WIDTH;
    background[alea][0] = CRGB::Green;
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

  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y < HEIGHT; y++)
    {
      if (y < etatFeu[x])
      {
        background[x][y] = CRGB::Black;
      }
      else
      {
        background[x][y].setHSV(map(constrain(y - etatFeu[x], 0, HEIGHT - 1), 0, HEIGHT - 1, 0, 60), 255, 255);
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
          bot[x * 8 - y + 15] = matrice[x][y];
        }
        else
        {
          bot[x * 8 + y - 8] = matrice[x][y];
        }
      }
      else
      {
        if (x % 2)
        {
          top[x * 8 - y + 7] = matrice[x][y];
        }
        else
        {
          top[x * 8 + y] = matrice[x][y];
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
        leds[x * 8 - y + 7] = matrice[x][y];
      }
      else
      {
        leds[x * 8 + y] = matrice[x][y];
      }
    }
  }
#endif
}

void overlayBuilder(bool pattern[], int width, int height, int xOffset, int yOffset)
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

void NaNoverlay(int originX, int originY)
{
#if FORMAT == 1
  overlayBuilder(N, 6, 6, originX, originY);
  overlayBuilder(a, 5, 6, originX + 7, originY);
  overlayBuilder(N, 6, 6, originX + 13, originY);
  overlayBuilder(o, 5, 6, originX + 20, originY);
#else
  overlayBuilder(N, 6, 8, originX, originY);
  overlayBuilder(a, 5, 6, originX + 7, originY + 2);
  overlayBuilder(N, 6, 8, originX + 13, originY);
  overlayBuilder(o, 5, 6, originX + 20, originY + 2);
#endif
}

#if FORMAT == 1
int overlayState = 0;
void nanoScroll()
{
  if (overlayState <= 0)
    overlayState = 35;
  NaNoverlay(overlayState, 1);
  NaNoverlay(overlayState - 35, 1);
  overlayState--;
}
#endif

void buildMatrice()
{
  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y < HEIGHT; y++)
    {
      if (!overlay[x][y][0] && !overlay[x][y][1] && !overlay[x][y][2])
      {
        matrice[x][y] = background[x][y];
      }
      else
      {
        matrice[x][y] = overlay[x][y];
      }
    }
  }
}