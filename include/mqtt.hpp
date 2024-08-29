#ifndef MQTT_H
#define MQTT_H

#include <ArduinoJson.h>
#include <Preferences.h>

#include "DisplayAssembly.hpp"
#include "MQTTClient.hpp"

void modeCallback(char *message, char *topic);

void brightnessCallback(char *message, char *topic);

void setupMqtt(MQTTClient *mqttClient, DisplayAssembly *disp,
               Preferences *preferences, short *bg, short *brightness);

#endif  // MQTT_H