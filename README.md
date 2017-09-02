MQTT Wifi SPIFFS Thing utility

# Purpose #
Implement sensors and actuators on MQTT in just a few lines.

# Features #
- Selects the access point with the best signal automatically.
- Reconnects when the access point disconnects.
- Stores access points settings on SPIFFS.
- Stores MQTT settings on SPIFFS.
- Uses lambda for callbacks.
- Uses a generic Value class as data type.

# Usage #
Create a data folder with two files: mqtt.txt and wifi.txt.
Write mqtt settings in mqtt.txt
1st line: server
2nd line: port
3rd line: username
4th line: password

Write access point settings in wifi.txt
odd lines: ssid
even lines: password

Set callback onStateChange.

Add sensor and actuator topic callbacks.

Call begin() inside setup()
Call handle() inside loop()

# Example #
```
#include "Thing.h"

using namespace g3rb3n;

Thing thing;

int count = 0;

void setup()
{
  Serial.begin(230400);
  Serial.println();
  
  thing.onStateChange([](const String& msg){
    Serial.println(msg);
  });
  
  thing.addSensor("sensor/test", 1000, [](Value& v){
    v = count++;
  });
  
  thing.addActuator("display/test", [](Value& v){
    String msg = v; 
    Serial.println(msg);
  });
  
  thing.begin();
}

void loop()
{
  thing.handle();
}
```
