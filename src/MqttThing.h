#ifndef _MqttThing_H
#define _MqttThing_H


#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <functional>
#include <vector>

#include "Value.h"

namespace g3rb3n
{

  struct SensorTopic
  {
    String name;
    int interval;
    long last;
    std::function<void(Value&)> function;
  };

  struct ActuatorTopic
  {
    String name;
    std::function<void(Value&)> function;
  };


  class MqttThing
  {
    public:

    private:
      static std::vector<MqttThing*> things;
      static void callback(char* callbackTopic, uint8_t* buffer, unsigned int length);

      String server;
      uint16_t port;
      String client;
      String user;
      String password;

      PubSubClient* pubSubClient;
      WiFiClient* wifiClient;

      std::function<void(const String&)> stateChangeCallback;

    public:
      MqttThing();
      MqttThing(bool useTLS);
      MqttThing(const char* _server, uint16_t _port, const char* _client, const char* _user, const char* _password, bool useTLS);
      ~MqttThing();

      void setServer(String& server, uint16_t port, bool useTLS);
      void setCredentials(String& username, String& password);
      void setClient(String& client);

      String& clientId();

      void publish(String& name, Value& value);
      void addSensor(String& topic, int interval, std::function<void(Value&)> f);
      void addSensor(char* topic, int interval, std::function<void(Value&)> f);
      void addActuator(String& topic, std::function<void(Value&)> cb);
      void addActuator(char* topic, std::function<void(Value&)> cb);

      void onStateChange(std::function<void(const String&)> callback);

      void begin();
      void handle();

      friend void callback(char* callbackTopic, uint8_t* buffer, unsigned int length);
      std::vector<SensorTopic> sensorTopics;
      std::vector<ActuatorTopic> actuatorTopics;

    private:
      void setDefaultClientId();
      void stateChange(const String& msg);
      void connect();

  };
}

#endif
