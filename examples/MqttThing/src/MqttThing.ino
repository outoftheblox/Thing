#include "MqttThing.h"
#include "WifiThing.h"

#include <Arduino.h>

using namespace g3rb3n;

#define SERVER "mqtt.outoftheblox.com"
#define PORT 8883
#define USERNAME "user"
#define PASSWORD "pass"
#define CLIENTID "test"

#define SSID "ssid"
#define PASS "pass"

#define SENSOR "test/count"
#define ACTUATOR "test/display"

WifiThing wifi;
MqttThing mqtt(SERVER, PORT, CLIENTID, USERNAME, PASSWORD, true);

int count = 0;

void setup()
{
  Serial.begin(230400);
  Serial.println();

  wifi.onStateChange([](const String& msg){
    Serial.print("wifi:" + msg);
  });
  mqtt.onStateChange([](const String& msg){
    Serial.println("mqtt:" + msg);
  });

  wifi.addAccessPoint(SSID, PASS);

  mqtt.addSensor(SENSOR, 1000, [](Value& v){
    v = count++;
  });
  mqtt.addActuator(ACTUATOR, [](Value& v){
    Serial.println(v);
  });

  wifi.begin();
  mqtt.begin();
}

void loop()
{
  wifi.handle();
  if (wifi.connected()) mqtt.handle();
}
