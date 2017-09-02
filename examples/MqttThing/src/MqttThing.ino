#include "MqttThing.h"
#include "WifiThing.h"

#include <Arduino.h>

using namespace g3rb3n;

#define SERVER "rfln.ddns.net"
#define PORT 1883
#define USERNAME "user@server"
#define PASSWORD "password"
#define CLIENTID "test"

#define SSID "ssid"
#define PASS "pass"

#define SENSOR "sensor/test"
#define ACTUATOR "display/test"

WifiThing wifi;
MqttThing mqtt(SERVER, PORT, CLIENTID, USERNAME, PASSWORD);

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

  wifi.addAccessPoint(SSID, PASS);
  
  mqtt.addSensor(SENSOR, 1000, [](Value& v){
    v = count++;
  });
  mqtt.addActuator(ACTUATOR, [](Value& v){
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
