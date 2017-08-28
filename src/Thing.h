#ifndef _THING_H
#define _THING_H

#include "Value.h"
#include "Function.h"
#include <vector>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <functional>
#include <algorithm>
//#include <vector>
using namespace gve;

struct SensorTopic {
  char name[40];
  int interval;
  long last;
  void (*function)(float&);
};

struct SensorTopic2 {
  char name[40];
  int interval;
  long last;
  std::function<void(Value&)> function;
};

struct ActuatorTopic {
  char name[40];
  void (*function)(float&);
};

struct Accesspoint {
  char* ssid;
  char* password;
};

static void callback(char* callbackTopic, uint8_t* buffer, unsigned int length);

class Thing {

  public:
    Thing();
    ~Thing();

    void chipID(std::string &name);
    void setMQTT(const char* server, int port, const char* client, const char* user, const char* password);
    void addWiFi(const char* ssid, const char* pwd);

    void handle();

    void setupWiFi();

    void reconnect();

    void onActivity(void (*fun)(bool));
    void onStateChange(void (*fun)(const String&));

    void addSensorTopic(const char* topic, int interval, std::function<void(Value&)> f);
    void addSensorTopic(const char* topic, int interval, void (*function)(float&));
    void addActuatorTopic(const char* topic, void (*function)(float&));

    Function& operator[](const std::string& name);
    Function& operator[](const char* name);
    
    friend Thing& operator<< (Thing& thing, std::string msg){
      std::string::size_type pos;
      pos = msg.find(':',0);
      std::string topic = msg.substr(0,pos);
      std::string value = msg.substr(pos+1);
      Value v = value;
      thing[topic](v);
    }
    
  private:
    void (*activity)(bool);
    void (*stateChange)(const String&);

    int wifi_timeout;
    std::vector<Accesspoint> accesspoints;

    char* mqtt_client;
    char* mqtt_server;
    int mqtt_port;
    char* mqtt_user;
    char* mqtt_password;

    friend void callback(char* callbackTopic, uint8_t* buffer, unsigned int length);
    std::vector<SensorTopic> sensor_topics;
    std::vector<SensorTopic2> sensor_topics2;
    std::vector<ActuatorTopic> actuator_topics;

    std::vector<Function> topics;
    
    WiFiClient wiFiClient;
    PubSubClient pubSubClient;

    Function& find(const std::string& name);
    bool contains(const std::string& name);
};
#endif
