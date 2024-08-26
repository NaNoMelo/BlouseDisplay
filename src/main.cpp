#include <Arduino.h>
#include <Preferences.h>
#include <stdlib.h>
#include <time.h>

#include "Debouncer.hpp"
#include "DisplayAssembly.hpp"
#include "DisplayCTL.hpp"
#include "MQTTClient.hpp"
#include "background.hpp"
#include "env.h"
#include "mqtt.hpp"
#include "overlay.hpp"

Preferences preferences;
DisplayAssembly *display;
MQTTClient *mqttClient;

short bg = 1;
short brightness = 1;

#define FPS 30

#define BG_BUTTON_PIN 19
#define BRIGHTNESS_BUTTON_PIN 18

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  preferences.begin("display");
  brightness = preferences.getShort("brightness", 1);
  bg = preferences.getShort("bg", 1);

  display = new DisplayAssembly();
  display->addController(32, 8, 27, VERTICAL, 0, 0);
  display->addController(32, 8, 25, VERTICAL, 0, 8);

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  FastLED.setMaxRefreshRate(60);
  brightness ? FastLED.setBrightness(1) : FastLED.setBrightness(16);
  FastLED.show();

  srand(time(NULL));
  pinMode(BG_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BUTTON_PIN, INPUT_PULLUP);

  mqttClient = new MQTTClient(WIFI_SSID, WIFI_PASS, MQTT_HOST, MQTT_PORT,"blouse",
                              MQTT_USER, MQTT_PASS);
  setupMqtt(mqttClient, display, &bg, &brightness);
}

Debouncer bg_button(BG_BUTTON_PIN);
Debouncer brightness_button(BRIGHTNESS_BUTTON_PIN);
void loop() {
  mqttClient->loop();
  brightness ? FastLED.setBrightness(1) : FastLED.setBrightness(16);

  bg_button.read();
  if (bg_button.isFallingEdge()) {
    bg = (bg + 1) % 4;
  }
  if (bg_button.isRisingEdge()) {
    preferences.putShort("bg", bg);
  }

  brightness_button.read();
  if (brightness_button.isFallingEdge()) {
    brightness = !brightness;
  }
  if (brightness_button.isRisingEdge()) {
    preferences.putShort("brightness", brightness);
  }

  static long lastFrame = 0;
  if (millis() - lastFrame > 50) {
    lastFrame = millis();

    switch (bg) {
      case 0:
        matriceRgb(display, brightness);
        NaNoverlay(display, 3, 4);
        break;
      case 1:
        matrix(display);
        NaNoverlay(display, 3, 4);
        break;
      case 2:
        fire(display);
        NaNoverlay(display, 3, 4);
        break;
      case 3:
        epilepsie(display);
        NaNoverlay(display, 3, 4);
        break;
      case 4:

      default:
        break;
    }
    FastLED.show();
  }
  /*// TEST SEGMENT ---------------------------------
  static unsigned short hue = 0;
  long time = millis();
  for (int i = 0; i < display.getWidth(); i++) {
    for (int j = 0; j < display.getHeight(); j++) {
      display.setPixel(i, j, CHSV((hue + i + j) % 256, 255, 255));
    }
  }
  hue++;
  FastLED.show();
  Serial.print("time:");
  Serial.println(millis() - time);
  delay(50);
  */// TEST SEGMENT ---------------------------------
}