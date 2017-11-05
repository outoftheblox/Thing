#include "MqttSpiffsThing.h"
#include "WifiSpiffsThing.h"

using namespace g3rb3n;

MqttSpiffsThing mqtt("/mqtt.txt");
WifiSpiffsThing wifi("/wifi.txt");

int count = 0;

void setup()
{
  Serial.begin(230400);
  Serial.println();

  wifi.onStateChange([](const String& msg){
    Serial.println("wifi:" + msg);
  });
  mqtt.onStateChange([](const String& msg){
    Serial.println("mqtt:" + msg);
  });

  wifi.begin();
  mqtt.begin();

  mqtt.addSensor(mqtt.clientId() + "/test/count", 1000, [](Value& v){
    v = count++;
  });
  mqtt.addActuator(mqtt.clientId() + "/test/display", [](Value& v){
    Serial.println(v);
  });

  Serial.println(mqtt.clientId());
}

void loop()
{
  wifi.handle();
  if (wifi.connected()) mqtt.handle();
}
