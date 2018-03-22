#include "Thing.h"

using namespace ootb;

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
