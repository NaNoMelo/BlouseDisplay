#ifndef MQTT_H
#define MQTT_H

#include <ArduinoJson.h>

#include <string>

#include "DisplayAssembly.hpp"
#include "MQTTClient.hpp"

void modeCallback(char *message, char *topic);

void brightnessCallback(char *message, char *topic);

void setupMqtt(MQTTClient *mqttClient, DisplayAssembly *disp, short *bg,
               short *brightness);

#endif  // MQTT_H