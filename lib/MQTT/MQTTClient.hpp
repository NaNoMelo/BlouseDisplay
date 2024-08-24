#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <PubSubClient.h>
#include <WiFi.h>

using namespace std;

struct MQTTSub {
  const char *topic;
  function<void(char*)> callback;
  MQTTSub *next = NULL;

  MQTTSub(const char *topic, function<void(char*)> callback)
      : topic(topic), callback(callback) {}
};

class MQTTClient {
 public:
  MQTTClient() = delete;
  MQTTClient(const char *wifiSsid, const char *wifiPass, const char *mqttHost,
             const char *mqttId, const char *mqttUser = "",
             const char *mqttPass = "");
  ~MQTTClient() = default;

  MQTTClient(const MQTTClient &) = delete;
  MQTTClient &operator=(const MQTTClient &) = delete;

  void loop();

 private:
  const char *_wifiSsid, *_wifiPass;
  const char *_mqttHost, *_mqttId, *_mqttUser, *_mqttPass;

  WiFiClient wifiClient;
  PubSubClient mqttClient;

  MQTTSub *subs = NULL;

  void setupWifi();
  void checkWifi(bool forceReconnect = false, bool setup = false);

  void mqttConnect();
  void mqttSubscribe();
  void mqttSubscribe(const char *topic, function<void(char*)> &callback);

  void handleMessage(char *topic, byte *payload, unsigned int length);
  bool topicMatch(const char *topic, const char *sub);
};

#endif  // MQTT_CLIENT_H