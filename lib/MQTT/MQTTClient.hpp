#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <PubSubClient.h>
#include <WiFi.h>

struct MQTTSub {
  char *topic;
  std::function<void(char *, char *)> callback;
  MQTTSub *next = NULL;

  MQTTSub(char *topic, std::function<void(char *, char *)> callback,
          MQTTSub *next)
      : topic(topic), callback(callback), next(next) {};

  ~MQTTSub() {
    if (next != NULL) delete next;
  }
};

class MQTTClient {
 public:
  MQTTClient() = delete;
  MQTTClient(const char *wifiSsid, const char *wifiPass, const char *mqttHost,
             const int mqttPort, const char *mqttId, const char *mqttUser,
             const char *mqttPass);
  ~MQTTClient() {
    if (subs != NULL) delete subs;
  };

  MQTTClient(const MQTTClient &) = delete;
  MQTTClient &operator=(const MQTTClient &) = delete;

  void loop();
  void subscribe(char *topic, std::function<void(char *, char *)> callback);

  void publish(char *topic, char *message, boolean retained = false);

 private:
  const char *_wifiSsid, *_wifiPass;
  const char *_mqttHost, *_mqttId, *_mqttUser, *_mqttPass;
  const int _mqttPort = 1883;

  WiFiClient wifiClient;
  PubSubClient mqttClient;

  MQTTSub *subs = NULL;

  void setupWifi();
  void checkWifi(bool forceReconnect = false, bool setup = false);

  void mqttConnect();
  void subscribe();

  void handleMessage(char *topic, byte *payload, unsigned int length);
  bool topicMatch(char *topic, char *sub);
};

#endif  // MQTT_CLIENT_H