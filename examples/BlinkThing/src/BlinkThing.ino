#include "Thing.h"
#include "BlinkPattern.h"

using namespace g3rb3n;

Thing thing;
BlinkPattern blink(BUILTIN_LED);

BlinkPattern::Pattern<4> disconnected{{1,1,1,6}, 100};
BlinkPattern::Pattern<2> connecting{{1,1}, 500};
BlinkPattern::Pattern<2> connected{{1,9}, 100};
BlinkPattern::Pattern<10> panic{{1,1,1,1,1,1,1,1,1,11}, 50};

int count = 0;

void setup()
{
  Serial.begin(230400);
  Serial.println();
  
  thing.onStateChange([](const String& msg){
    WifiThing::State state = thing.state();
    
    switch(state)
    {
      case WifiThing::State::Disconnected: blink.setPattern(disconnected); break;
      case WifiThing::State::Connecting: blink.setPattern(connecting); break;
      case WifiThing::State::Connected: blink.setPattern(connected); break;
      default: blink.setPattern(panic);
    }
    Serial.println(msg);
  });

  thing.addSensor("sensor/test", 1000, [](Value& v){
    v = count++;
  });
  thing.addActuator("display/test", [](Value& v){
    String msg = v; 
    Serial.println(msg);
  });
  
  blink.begin();
  thing.begin();
}

void loop()
{
  blink.handle();
  thing.handle();
}
