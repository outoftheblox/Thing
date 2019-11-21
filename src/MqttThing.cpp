#include "MqttThing.h"

#include <vector>
#include <functional>
#include <ESP8266WiFi.h>

using namespace ootb;

std::vector<MqttThing*> MqttThing::things;

void MqttThing::callback(char* callbackTopic, uint8_t* buffer, unsigned int length)
{
  char msg[length+1];
  for (int i = 0; i < length; i++)
  {
    msg[i] = (char)buffer[i];
  }
  msg[length] = 0;
  Value value = msg;
  for (std::vector<MqttThing*>::iterator it = things.begin() ; it != things.end(); ++it)
  {
    MqttThing& thing = (**it);
    for(auto & topic: (*it)->actuatorTopics)
    {
      if (topic.name.equals(callbackTopic))
      {
        topic.function(value);
      }
    }
  }
}

void MqttThing::setDefaultClientId()
{
  uint8_t arr[6];
  WiFi.macAddress(arr);
  for (byte i = 0; i < 6; ++i)
  {
    char buf[3];
    sprintf(buf, "%02x", arr[i]);
    client += buf;
    //if (i < 5) client += ':';
  }
}

MqttThing::MqttThing()
{
  wifiClient = NULL;
  pubSubClient = NULL;
  Serial.println("MqttThing()");
  things.push_back(this);
  setDefaultClientId();
}

MqttThing::MqttThing(bool useTLS)
{
  if (!useTLS)
    wifiClient = new WiFiClientSecure();
  else
    wifiClient = new WiFiClient();

  pubSubClient = new PubSubClient(*wifiClient);
  things.push_back(this);
  pubSubClient->setServer(server.c_str(), port);
  setDefaultClientId();
}

MqttThing::MqttThing(const char* _server, uint16_t _port, const char* _client, const char* _user, const char* _password, bool useTLS)
:
server(String(_server)),
port(_port),
client(String(_client)),
user(String(_user)),
password(String(_password))
{
  if (useTLS)
    wifiClient = new WiFiClientSecure();
  else
    wifiClient = new WiFiClient();
  pubSubClient = new PubSubClient(*wifiClient);
  things.push_back(this);
  pubSubClient->setServer(server.c_str(), port);
}

MqttThing::~MqttThing()
{
  for (std::vector<MqttThing*>::iterator it = things.begin() ; it != things.end(); ++it)
  {
    if ((*it) == this)
      things.erase(it, it+1);
  }
}

void MqttThing::publish(String& name, Value& value)
{
  if (value.type() == Value::t_bool){
    String s((bool)value);
    pubSubClient->publish(name.c_str(), s.c_str());
  }
  if (value.type() == Value::t_char){
    String s((char)value);
    pubSubClient->publish(name.c_str(), s.c_str());
  }
  if (value.type() == Value::t_short){
    String s((short)value);
    pubSubClient->publish(name.c_str(), s.c_str());
  }
  if (value.type() == Value::t_int){
    String s((int)value);
    pubSubClient->publish(name.c_str(), s.c_str());
  }
  if (value.type() == Value::t_long){
    String s((long)value);
    pubSubClient->publish(name.c_str(), s.c_str());
  }
  if (value.type() == Value::t_longlong){
    String s((long)value);
    pubSubClient->publish(name.c_str(), s.c_str());
  }
  if (value.type() == Value::t_float){
    String s((float)value, 7);
    pubSubClient->publish(name.c_str(), s.c_str());
  }
  if (value.type() == Value::t_double){
    String s((double)value, 13);
    pubSubClient->publish(name.c_str(), s.c_str());
  }
  if (value.type() == Value::t_longdouble){
    String s((double)value, 26);
    pubSubClient->publish(name.c_str(), s.c_str());
  }
  if (value.type() == Value::t_cstring){
    String s(value);
    pubSubClient->publish(name.c_str(), s.c_str());
  }
}

void MqttThing::setServer(String& _server, uint16_t _port, bool useTLS)
{
  if (useTLS)
    wifiClient = new WiFiClientSecure();
  else
    wifiClient = new WiFiClient();
  pubSubClient = new PubSubClient(*wifiClient);

  server = _server;
  port = _port;
  pubSubClient->setServer(server.c_str(), port);
}

void MqttThing::setCredentials(String& _username, String& _password)
{
  user = _username;
  password = _password;
}

void MqttThing::setClient(String& _client)
{
  client = _client;
}

String& MqttThing::clientId()
{
  return client;
}

void MqttThing::addSensor(char* topic, int interval, std::function<void(Value&)> f)
{
  String t(topic);
  addSensor(t, interval, f);
}

void MqttThing::addSensor(String& name, int interval, std::function<void(Value&)> function)
{
  SensorTopic t;
  t.name = name;
  t.interval = interval;
  t.function = function;
  t.last = millis() - interval;
  sensorTopics.push_back(t);
}

void MqttThing::addActuator(char* topic, std::function<void(Value&)> f)
{
  String t(topic);
  addActuator(t, f);
}

void MqttThing::addActuator(String& name, std::function<void(Value&)> function){
  this->pubSubClient->subscribe(name.c_str());
  this->pubSubClient->setCallback(callback);
  ActuatorTopic t;
  t.name = name;
  t.function = function;
  actuatorTopics.push_back(t);
}

void MqttThing::onStateChange(std::function<void(const String&)> callback)
{
  stateChangeCallback = callback;
}

void MqttThing::stateChange(const String& msg)
{
  if (stateChangeCallback)
    stateChangeCallback(msg);
}

void MqttThing::begin()
{
}

void MqttThing::handle()
{
  if (!pubSubClient)
  {
    if (stateChangeCallback) stateChangeCallback(String("Client is not initialized"));
    return;
  }
  if (!pubSubClient->connected())
  {
    if (stateChangeCallback) stateChangeCallback(String("Disconnected from MQTT server"));
    connect();
  }
  pubSubClient->loop();

  long now = millis();
  for(auto & topic: sensorTopics)
  {
    if (now - topic.last > topic.interval)
    {
      topic.last = now;
      Value value;
      topic.function(value);
      if (value.type() == Value::t_bool){
        String s((bool)value);
        pubSubClient->publish(topic.name.c_str(), s.c_str());
      }
      if (value.type() == Value::t_char){
        String s((char)value);
        pubSubClient->publish(topic.name.c_str(), s.c_str());
      }
      if (value.type() == Value::t_short){
        String s((short)value);
        pubSubClient->publish(topic.name.c_str(), s.c_str());
      }
      if (value.type() == Value::t_int){
        String s((int)value);
        pubSubClient->publish(topic.name.c_str(), s.c_str());
      }
      if (value.type() == Value::t_long){
        String s((long)value);
        pubSubClient->publish(topic.name.c_str(), s.c_str());
      }
      if (value.type() == Value::t_longlong){
        String s((long)value);
        pubSubClient->publish(topic.name.c_str(), s.c_str());
      }
      if (value.type() == Value::t_float){
        String s((float)value, 7);
        pubSubClient->publish(topic.name.c_str(), s.c_str());
      }
      if (value.type() == Value::t_double){
        String s((double)value, 13);
        pubSubClient->publish(topic.name.c_str(), s.c_str());
      }
      if (value.type() == Value::t_longdouble){
        String s((double)value, 26);
        pubSubClient->publish(topic.name.c_str(), s.c_str());
      }
      if (value.type() == Value::t_cstring){
        String s(value);
        pubSubClient->publish(topic.name.c_str(), s.c_str());
      }
    }
  }
}


void MqttThing::connect()
{
  // Loop until we're reconnected
  String msg("Connecting to MQTT server ");
  msg += server;
  stateChange(msg);

  if (!pubSubClient->connected())
  {
    if (!pubSubClient->connect(client.c_str(), user.c_str(), password.c_str()))
    {
      String msg("Connecting to MQTT server ");
      msg += server;
      msg += " failed.";
      stateChange(msg);
      return;
    }
/*
    if(!wifiClient.verifyCertChain(server.c_str()))
    {
      String msg("Could not verify domain name ");
      msg += server;
      stateChange(msg);
      return;
    }
*/
    for(auto & topic: actuatorTopics)
    {
      pubSubClient->subscribe(topic.name.c_str());
    }
    String msg("Connected to MQTT server ");
    msg += server;
    stateChange(msg);
  }
}
