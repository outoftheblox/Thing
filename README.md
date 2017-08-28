WiFi Thing utility

# Example #
```
#define MQTT_SERVER "ip"
#define MQTT_PORT port
#define MQTT_USER "user"
#define MQTT_PASSWORD "pass"

#define MQTT_CLIENT "example"
#define MQTT_DISPLAY_TOPIC "actuator/example"
#define MQTT_SENSOR_TOPIC "sensor/example"

//#define _DEBUG_

#include <Thing.h>

Thing thing;

void setup() {
  Serial.begin(230400);
  Serial.println();

  pinMode(BUILTIN_LED, OUTPUT);  

  onActivity(true);

  Serial.print("Client:");
  Serial.print(MQTT_CLIENT);
  Serial.println();
    
  thing.onActivity(&onActivity);
  thing.addWiFi(WIFI_SSID, WIFI_PASS);
  thing.setupWiFi();
  thing.setMQTT(MQTT_SERVER, MQTT_PORT, MQTT_CLIENT, MQTT_USER, MQTT_PASSWORD);

  thing.addTopic(SENSOR_TOPIC, 5000, getValue);
  Serial.println(SENSOR_TOPIC);

  thing.subscribe(ACTUATOR_TOPIC, callback);
  Serial.println(ACTUATOR_TOPIC);

  onActivity(false);
}

void loop() {
  thing.loop();
}

void onActivity(bool active){
  digitalWrite(BUILTIN_LED, active ? LOW : HIGH);  
}

void getValue(float& value)
{
  value = 100;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char msg[length+1];
  for (int i = 0; i < length; i++) {
    msg[i] = (char)payload[i];
  }
  msg[length] = 0;
  Serial.println(msg);
}

```
