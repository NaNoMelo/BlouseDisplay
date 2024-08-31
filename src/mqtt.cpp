#include "mqtt.hpp"

#include "overlay.hpp"

MQTTClient *client;

DisplayAssembly *mqttDisplay;

Preferences *preference;

short *background;
short *bright;

void modeCallback(char *message, char *topic) {
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Received: ");
  Serial.println(message);
  if (strcmp(message, "matricergb") == 0) {
    *background = 0;
  } else if (strcmp(message, "matrix") == 0) {
    *background = 1;
  } else if (strcmp(message, "fire") == 0) {
    *background = 2;
  } else if (strcmp(message, "epilepsie") == 0) {
    *background = 3;
  } else if (strcmp(message, "mqtt") == 0) {
    *background = 4;
    FastLED.clear(true);
  }
  preference->putShort("bg", *background);
}

void brightnessCallback(char *message, char *topic) {
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Received: ");
  Serial.println(message);
  *bright = atoi(message);
  preference->putShort("brightness", *bright);
}

void ledCallback(char *message, char *topic) {
  // parse topic for coords
  Serial.print("Topic: ");
  Serial.println(topic);
  char *base = (char *)"blouse/leds/";
  while (*base) {
    base++;
    topic++;
  }
  char *char_x = strtok(topic, "/");
  char *char_y = strtok(NULL, "/");
  int x = atoi(char_x);
  int y = atoi(char_y);
  if (x < mqttDisplay->getMinX() || x > mqttDisplay->getMaxX() ||
      y < mqttDisplay->getMinY() || y > mqttDisplay->getMaxY()) {
    return;
  }

  // decode color
  Serial.print("Received: ");
  Serial.println(message);
  JsonDocument color;
  switch (*message) {
    case '#':
      message++;
      mqttDisplay->setPixel(x, y, CRGB(std::stoi(message, 0, 16)));
      break;
    case '{':
      DeserializationError error = deserializeJson(color, message);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      if (color["R"] && color["G"] && color["B"]) {
        mqttDisplay->setPixel(x, y, CRGB(color["R"], color["G"], color["B"]));
      } else if (color["H"] && color["S"] && color["V"]) {
        mqttDisplay->setPixel(x, y, CHSV(color["H"], color["S"], color["V"]));
      } else {
        Serial.println("Invalid color format");
      }
      break;
  }
};

void ledsCallback(char *message, char *topic) {
  JsonDocument messageJson;
  DeserializationError error = deserializeJson(messageJson, message);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  if (messageJson["leds"]) {
    JsonArray leds = messageJson["leds"].as<JsonArray>();
    for (int i = 0; i < leds.size(); i++) {
      int x = leds[i]["x"];
      int y = leds[i]["y"];
      if (x < mqttDisplay->getMinX() || x > mqttDisplay->getMaxX() ||
          y < mqttDisplay->getMinY() || y > mqttDisplay->getMaxY()) {
        continue;
      }
      if (leds[i]["color"]) {
        const char *color = leds[i]["color"].as<const char *>();
        if (*color == '#') color++;
        mqttDisplay->setPixel(x, y, CRGB(std::stoi(color, 0, 16)));
      } else if (leds[i]["R"] && leds[i]["G"] && leds[i]["B"]) {
        mqttDisplay->setPixel(x, y,
                              CRGB(leds[i]["R"], leds[i]["G"], leds[i]["B"]));
      } else if (leds[i]["H"] && leds[i]["S"] && leds[i]["V"]) {
        mqttDisplay->setPixel(x, y,
                              CHSV(leds[i]["H"], leds[i]["S"], leds[i]["V"]));
      } else {
        Serial.println("Invalid color format");
      }
    }
  } else {
    int x = messageJson["x"];
    int y = messageJson["y"];
    if (x < mqttDisplay->getMinX() || x > mqttDisplay->getMaxX() ||
        y < mqttDisplay->getMinY() || y > mqttDisplay->getMaxY()) {
      return;
    }
    if (messageJson["color"]) {
      const char *color = messageJson["color"].as<const char *>();
      if (*color == '#') color++;
      mqttDisplay->setPixel(x, y, CRGB(std::stoi(color, 0, 16)));
    } else if (messageJson["R"] && messageJson["G"] && messageJson["B"]) {
      mqttDisplay->setPixel(
          x, y, CRGB(messageJson["R"], messageJson["G"], messageJson["B"]));
    } else if (messageJson["H"] && messageJson["S"] && messageJson["V"]) {
      mqttDisplay->setPixel(
          x, y, CHSV(messageJson["H"], messageJson["S"], messageJson["V"]));
    } else {
      Serial.println("Invalid color format");
    }
  }
}

void nanoCallback(char *message, char *topic) { NaNoverlay(mqttDisplay, 3, 4); }

void setupMqtt(MQTTClient *mqttClient, DisplayAssembly *disp,
               Preferences *preferences, short *bg, short *brightness) {
  client = mqttClient;
  mqttDisplay = disp;
  preference = preferences;
  background = bg;
  bright = brightness;

  mqttClient->subscribe((char *)"blouse/mode", modeCallback);
  mqttClient->subscribe((char *)"blouse/brightness", brightnessCallback);
  mqttClient->subscribe((char *)"blouse/leds/#", ledCallback);
  mqttClient->subscribe((char *)"blouse/leds", ledsCallback);
  mqttClient->subscribe((char *)"blouse/nanoverlay", nanoCallback);
}
