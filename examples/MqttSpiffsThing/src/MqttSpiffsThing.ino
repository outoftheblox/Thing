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
    Serial.print("wifi:");
    Serial.println(msg);}
  );
  mqtt.onStateChange([](const String& msg){
    Serial.print("mqtt:");
    Serial.println(msg);}
  );

  mqtt.addSensor("sensor/test", 1000, [](Value& v){
    v = count++;
  });
  mqtt.addActuator("display/test", [](Value& v){
    String msg = v; 
    Serial.println(msg);
  });

  wifi.begin();
  mqtt.begin();
}

void loop()
{
  wifi.handle();
  mqtt.handle();
}
