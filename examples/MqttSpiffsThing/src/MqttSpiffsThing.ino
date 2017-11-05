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

  mqtt.addSensor("sensor/test", 1000, [](Value& v){
    v = count++;
  });
  mqtt.addActuator("display/test", [](Value& v){
    Serial.println(v);
  });
}

void loop()
{
  wifi.handle();
  if (wifi.connected()) mqtt.handle();
}
