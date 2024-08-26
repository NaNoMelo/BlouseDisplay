#include "mqtt.hpp"

MQTTClient *client;

DisplayAssembly *mqttDisplay;

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
}

void brightnessCallback(char *message, char *topic) {
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Received: ");
  Serial.println(message);
  if (strcmp(message, "on") == 0) {
    *bright = 1;
  } else if (strcmp(message, "off") == 0) {
    *bright = 0;
  }
}

void ledsCallback(char *message, char *topic) {
  // parse topic
  Serial.print("Topic: ");
  Serial.println(topic);
  char *base = (char *)"blouse/leds/";
  while (*base) {
    base++;
    topic++;
  }
  Serial.print("topic: ");
  Serial.println(topic);
  char *char_x = strtok(topic, "/");
  char *char_y = strtok(NULL, "/");

  // parse message
  Serial.print("Received: ");
  Serial.println(message);
  JsonDocument color;
  DeserializationError error = deserializeJson(color, message);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // set pixel
  int x = atoi(char_x);
  int y = atoi(char_y);
  if (x < mqttDisplay->getMinX() || x > mqttDisplay->getMaxX() ||
      y < mqttDisplay->getMinY() || y > mqttDisplay->getMaxY()) {
    return;
  }
  mqttDisplay->setPixel(x, y, CRGB(color["R"], color["G"], color["B"]));
};

void setupMqtt(MQTTClient *mqttClient, DisplayAssembly *disp, short *bg,
               short *brightness) {
  client = mqttClient;
  mqttDisplay = disp;
  background = bg;
  bright = brightness;

  mqttClient->subscribe((char *)"blouse/leds/#", ledsCallback);
  mqttClient->subscribe((char *)"blouse/mode", modeCallback);
  mqttClient->subscribe((char *)"blouse/brightness", brightnessCallback);
}
