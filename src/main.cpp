#include <Arduino.h>
#include <FastLED.h>
#include <stdlib.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <string.h>

#define WIDTH 32
#define HEIGHT 16
#define BOT_LED_PIN 23
#define TOP_LED_PIN 22
#define SERVICE_UUID "83664ec8-0740-11ed-b939-0242ac120002"
#define CHARACTERISTIC_UUID "e86205ec-0740-11ed-b939-0242ac120002"

void matriceToLed();
void rgbNext(int nb);
void matriceRgb();
void matrix();
void NaNoverlay();

int NaNo[8][25] = {
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0},
    {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0}};

CRGB top[256];
CRGB bot[256];

int matrice[WIDTH][HEIGHT][3];
int background[WIDTH][HEIGHT][3];

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
  FastLED.addLeds<NEOPIXEL, TOP_LED_PIN>(top, 8 * 32);
  FastLED.addLeds<NEOPIXEL, BOT_LED_PIN>(bot, 8 * 32);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  FastLED.setMaxRefreshRate(0);
  srand(time(NULL));
  // FastLED.setBrightness(64);

  /*BLEDevice::init("Blouse");
  BLEServer *pServer = BLEDevice::createServer();
  // pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pService->addCharacteristic(&presetCharacteristics);
  presetDescriptor.setValue("Preset");
  presetCharacteristics.addDescriptor(&presetDescriptor);
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();*/
}

int r = 255, g = 0, b = 0;
int pr, pg, pb;
// std::__cxx11::string preset = presetCharacteristics.getValue();
int mode = 1;
int timer = 1000;
void loop()
{
  // mode = stoi(preset);
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
  default:
    break;
  }
  NaNoverlay();
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
          background[x][y][0] = r / 4, background[x][y][1] = g / 4, background[x][y][2] = b / 4;
        }
      }
    }
    rgbNext(8);
  }
}

int alea;
void matrix()
{
  for (int y = 15; y > 0; y--)
  {
    for (int x = 0; x < 32; x++)
    {
      background[x][y][0] = background[x][y - 1][0];
      background[x][y][1] = background[x][y - 1][1];
      background[x][y][2] = background[x][y - 1][2];
    }
  }
  for (int x = 0; x < 32; x++)
  {
    background[x][0][0] = 0;
    background[x][0][1] = 0;
    background[x][0][2] = 0;
  }
  for (int i = 0; i < 3; i++)
  {
    alea = rand() % 32;
    background[alea][0][0] = 0;
    background[alea][0][1] = 255;
    background[alea][0][2] = 0;
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
  for (int x = 0; x < 32; x++)
  {
    for (int y = 0; y < 16; y++)
    {
      matrice[x][y][0] = background[x][y][0];
      matrice[x][y][1] = background[x][y][1];
      matrice[x][y][2] = background[x][y][2];
      if (y > 3 && x > 2 && x < 25 + 3 && y < 8 + 4)
      {
        if (NaNo[y - 4][x - 3] == 1)
        {
          matrice[x][y][0] = 255;
          matrice[x][y][1] = 255;
          matrice[x][y][2] = 255;
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