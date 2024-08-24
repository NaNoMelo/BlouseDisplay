
#include "MQTTClient.hpp"

MQTTClient::MQTTClient(const char *wifiSsid, const char *wifiPass,
                       const char *mqttHost, const char *mqttId,
                       const char *mqttUser = "", const char *mqttPass = "")
    : _wifiSsid(wifiSsid),
      _wifiPass(wifiPass),
      _mqttHost(mqttHost),
      _mqttId(mqttId),
      _mqttUser(mqttUser),
      _mqttPass(mqttPass) {
  setupWifi();
  mqttClient.setClient(wifiClient);
  mqttClient.setServer(_mqttHost, 1883);
  mqttClient.setCallback(
      [this](char *topic, byte *payload, unsigned int length) {
        this->handleMessage(topic, payload, length);
      });
}

void MQTTClient::loop() {
  // Wi-Fi Handling
  static bool lastWifiStatus = false;
  bool wifiStatus = WiFi.status() == WL_CONNECTED;

  if (wifiStatus != lastWifiStatus) {
    lastWifiStatus = wifiStatus;
    checkWifi(!wifiStatus);
  } else if (!wifiStatus) {
    checkWifi();
    return;
  }

  // MQTT Handling
  static bool lastMqttStatus = false;
  static unsigned long lastMqttStart = 0;
  bool mqttStatus = mqttClient.connected();
  static bool connectingMqtt = false;

  if (mqttStatus != lastMqttStatus) {
    lastMqttStatus = mqttStatus;
    if (!mqttStatus) {
      lastMqttStart = millis();
      connectingMqtt = true;
      mqttConnect();
    } else {
      connectingMqtt = false;
    }
  } else if (!mqttStatus && connectingMqtt) {
    static long lastMqttAttempt = 0;
    if (millis() - lastMqttAttempt > 5000) {
      lastMqttAttempt = millis();
      mqttConnect();
    }
    if (millis() - lastMqttStart > 20000) {
      Serial.println("Failed to connect to MQTT");
      mqttClient.disconnect();
      connectingMqtt = false;
    }
    return;
  }
  if (mqttStatus) mqttClient.loop();
}

void MQTTClient::setupWifi() { checkWifi(false, true); }

void MQTTClient::checkWifi(bool forceReconnect, bool setup) {
  static bool connecting = false;
  static unsigned long startAttemptTime = 0;

  if (forceReconnect || setup) {
    connecting = true;
    startAttemptTime = millis();
    Serial.print("Connecting to ");
    Serial.println(_wifiSsid);
    if (setup) {
      WiFi.begin(_wifiSsid, _wifiPass);
    } else {
      WiFi.reconnect();
    }
  }
  if (connecting) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connecting = false;
    } else {
      if (millis() - startAttemptTime > 20000) {
        Serial.println("Failed to connect to WiFi");
        WiFi.disconnect(true);
        connecting = false;
      }
    }
  }
}

void MQTTClient::mqttConnect() {
  Serial.print("Attempting MQTT connection...");
  if (mqttClient.connect(_mqttId, _mqttUser, _mqttPass)) {
    Serial.println("connected");
    mqttSubscribe();
  } else {
    Serial.print("failed, rc=");
    Serial.print(mqttClient.state());
    Serial.println(" try again in 5 seconds");
  }
}

void MQTTClient::mqttSubscribe() {
  if (!mqttClient.connected()) return;
  MQTTSub *sub = subs;
  while (sub != NULL) {
    mqttClient.subscribe(sub->topic);
  }
}

void MQTTClient::mqttSubscribe(const char *topic,
                               function<void(char *)> &callback) {
  subs = new MQTTSub(topic, callback);
  if (!mqttClient.connected()) return;
  mqttClient.subscribe(topic);
}

void MQTTClient::handleMessage(char *topic, byte *payload,
                               unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char message[length + 1];
  message[length] = '\0';
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();

  MQTTSub *sub = subs;
  while (sub != NULL) {
    if (topicMatch(topic, sub->topic)) {
      sub->callback((char *)payload);
    }
    sub = sub->next;
  }
}

bool MQTTClient::topicMatch(const char *topic, const char *sub) {
  while (*topic && *sub) {
    if (*sub == '+') {
      sub++;
      while (*topic && *topic != '/') {
        topic++;
      }
    } else if (*sub == '#') {
      return true;
    }

    if (*topic != *sub) {
      return false;
    }
    if (*topic) topic++;
    if (*sub) sub++;
  }
  return (*topic == *sub);
}
