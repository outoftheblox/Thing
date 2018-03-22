#include "MqttSpiffsThing.h"
#include "WifiSpiffsThing.h"

using namespace ootb;

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

  mqtt.addSensor(String("test/count/") + mqtt.clientId(), 1000, [](Value& v){
    v = count++;
  });
  mqtt.addActuator(String("/test/display/") + mqtt.clientId(), [](Value& v){
    Serial.println(v);
  });

  Serial.println(mqtt.clientId());
}

void loop()
{
  wifi.handle();
  if (wifi.connected()) mqtt.handle();
}
