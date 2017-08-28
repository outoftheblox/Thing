//
// Copyright (c) 2017 
// Author: 3grbn3@gmail.com (Gerben)
//
// A combination of WiFi MQTT and function pointers / lambda
//

#include "Thing.h"

#include <Arduino.h>
//#include <iostream>
#include <sstream>
//#include <String.h>

std::vector<Thing*> things;

static void callback(char* callbackTopic, uint8_t* buffer, unsigned int length){
  char msg[length+1];
  for (int i = 0; i < length; i++) {
    msg[i] = (char)buffer[i];
  }
  msg[length] = 0;
  float value = atof(msg);
  for (std::vector<Thing*>::iterator it = things.begin() ; it != things.end(); ++it){
    Thing& thing = (**it);
    for(auto & topic: (*it)->actuator_topics) {
      if (strcmp(callbackTopic, topic.name) == 0){
        if (thing.activity)
          thing.activity(true);
        topic.function(value);
        if (thing.activity)
          thing.activity(false);
      }
    }
  }
}

Thing::Thing(){
  this->pubSubClient = PubSubClient(this->wiFiClient);
  this->activity = 0;
  this->wifi_timeout = 10;
  things.push_back(this);
}

Thing::~Thing(){
  for (std::vector<Thing*>::iterator it = things.begin() ; it != things.end(); ++it){
    if ((*it) == this)
      things.erase(it, it+1);
  }
}

void Thing::chipID(std::string &name){
  char chipid[40];
  sprintf(chipid, "%08X", ESP.getChipId());
  name += chipid;
}

void Thing::addWiFi(const char* ssid, const char* password){
  Accesspoint ap;
  ap.ssid = (char*)ssid;
  ap.password = (char*)password;
  this->accesspoints.push_back(ap);
}

void Thing::setMQTT(const char* server, int port, const char* client, const char* user, const char* password){
  this->mqtt_client = (char*)client;
  this->mqtt_server = (char*)server;
  this->mqtt_port = port;
  this->mqtt_user = (char*)user;
  this->mqtt_password = (char*)password;
  this->pubSubClient.setServer(this->mqtt_server, this->mqtt_port);
}

void Thing::onActivity(void (*fun)(bool)){
  this->activity = fun;
}

void Thing::onStateChange(void (*fun)(const String&)){
  this->stateChange = fun;
}

void Thing::addSensorTopic(const char* name, int interval, void (*function)(float&)){
  SensorTopic t;
  strcpy(t.name, name);
  t.interval = interval;
  t.function = function;
  t.last = millis() - interval;
  this->sensor_topics.push_back(t);
}

void Thing::addSensorTopic(const char* name, int interval, std::function<void(Value&)> cb){
  Function f(name);
  f << interval;
  f << cb;
  this->topics.push_back(f);
}

void Thing::addActuatorTopic(const char* name, void (*function)(float&)){
  this->pubSubClient.subscribe(name);
  this->pubSubClient.setCallback(callback);
  ActuatorTopic t;
  strcpy(t.name, name);
  t.function = function;
  this->actuator_topics.push_back(t);
}

Function& Thing::operator[](const std::string& name){
  if (!contains(name)){
    Function function(name);
    topics.push_back(function);
  }
  return find(name);
}

Function& Thing::operator[](char const* name_){
  std::string name;
  name = name_;
  return (*this)[name];
}

Function& Thing::find(const std::string& name){
  std::vector<Function>::iterator it = std::find_if(topics.begin(), topics.end(),
         [&name](Function & t) -> bool { return ! name.compare(t.name()); });
    return *it;
}

bool Thing::contains(const std::string& name){
  std::vector<Function>::iterator it = std::find_if(topics.begin(), topics.end(),
         [&name](Function & t) -> bool { return ! name.compare(t.name()); });
    return it != topics.end();
}

void Thing::handle(){
  if (!this->pubSubClient.connected()) {
    if (this->stateChange) this->stateChange(String("Disconnected"));
    this->reconnect();
  }
  this->pubSubClient.loop();
  long now = millis();
  for(auto & topic: this->sensor_topics) {
    if (now - topic.last > topic.interval){
      if (this->activity != 0)
        this->activity(true);
      topic.last = now;
      float value;
      topic.function(value);
      char result[10];
      dtostrf(value, 9, 3, result);
      this->pubSubClient.publish(topic.name, result, true);
      if (this->activity != 0)
        this->activity(false);
//      return;
   //    this->pubSubClient.publish(mqtt_topic, String(value).c_str(), true);
    }
  }
  for(auto & topic: this->topics) {
    if (now - topic.last() > topic.interval()){
      if (this->activity != 0)
        this->activity(true);
      topic.last(now);
      Value value;
      topic(value);
//      std::stringstream ss;
//      ss << value;
//      std::string s = ss.str();
      if (value.type() == Value::t_float){
        String s((float)value, 7);
        this->pubSubClient.publish(topic.name().c_str(), s.c_str(), true);
      }
      if (value.type() == Value::t_int){
        String s((int)value);
        this->pubSubClient.publish(topic.name().c_str(), s.c_str(), true);
      }
      if (this->activity != 0)
        this->activity(false);
    }
  }
}

void Thing::setupWiFi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();

  for(auto & ap: this->accesspoints) {
    WiFi.begin(ap.ssid, ap.password);
    int i = 0;
    if (this->stateChange)
    {
      String state("Connecting to ");
      state += ap.ssid;
      this->stateChange(state);
    }
    
    Serial.print("Connecting to ");
    Serial.print(ap.ssid);
    while (WiFi.status() != WL_CONNECTED) {
      if (this->activity)
        this->activity(true);
      if (++i > this->wifi_timeout){
        Serial.println("timed out");
        if (this->activity){
          for (int i = 0 ; i < 5 ; ++i){
            delay(50);
            this->activity(true);
            delay(50);
            this->activity(false);
          }
        }
        break;
      }
      delay(500);
      if (this->activity)
        this->activity(false);
      delay(500);
      Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED){
      if (this->stateChange)
      {
        String state("Connected to ");
        state += ap.ssid;
        this->stateChange(state);
      }
      if (this->activity){
        for (int i = 0 ; i < 2 ; ++i){
          delay(100);
          this->activity(true);
          delay(100);
          this->activity(false);
        }
      }
      break;
    }
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (this->stateChange)
  {
    String state("IP is ");
    state += WiFi.localIP();
    this->stateChange(state);
  }
}
/*
void Thing::callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  this->mqtt_input_callback((char*)payload);
  Serial.println();
}
*/

/*
void Thing::subscribe(char* topic, void (*fun)(char*)){
  this->pubSubClient.subscribe(topic);// - See more at: http://www.esp8266.com/viewtopic.php?f=29&t=8746#sthash.IcU9WetT.dpuf
  this->pubSubClient.setCallback(callback);
  this->mqtt_input_topic = topic;
  this->mqtt_input_callback = fun;
}
*/
void Thing::reconnect() {
  // Loop until we're reconnected
  if (this->stateChange)
  {
    String state("Reconnecting to ");
    state += this->mqtt_server;
    this->stateChange(state);
  }

  while (!this->pubSubClient.connected()) {
    char* cp = this->mqtt_client;
    Serial.printf("Attempting MQTT connection to %s as %s...", this->mqtt_server, this->mqtt_client);
    if (this->pubSubClient.connect(this->mqtt_client, this->mqtt_user, this->mqtt_password)) {
      for(auto & topic: this->actuator_topics) {
        this->pubSubClient.subscribe(topic.name);// - See more at: http://www.esp8266.com/viewtopic.php?f=29&t=8746#sthash.IcU9WetT.dpuf
      }
      Serial.println("connected");
      if (this->stateChange)
      {
        String state("Connected to MQTT server ");
        state += this->mqtt_server;
        this->stateChange(state);
      }
    } else {
      if (this->stateChange)
      {
        String state("Connecting to ");
        state += this->mqtt_server;
        state += " failed. Retrying in 5 seconds.";
        this->stateChange(state);
      }
      Serial.print("failed, rc=");
      Serial.print(this->pubSubClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    this->mqtt_client = cp;
  }
}
