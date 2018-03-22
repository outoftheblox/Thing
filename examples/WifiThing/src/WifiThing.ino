#include "WifiThing.h"

#include <algorithm>

using namespace ootb;

WifiThing wifi;

void setup()
{
  Serial.begin(230400);
  Serial.println();
  
  wifi.onStateChange([](const String& msg){
    Serial.println(msg);
  });

  wifi.addAccessPoint("ssid1", "pass1");
  wifi.addAccessPoint("ssid2", "pass2");

  wifi.begin();

  std::for_each(
    wifi.accessPoints().begin(), 
    wifi.accessPoints().end(), 
    [](AccessPoint& ap){
      Serial.print(ap.ssid);
      Serial.print(' ');
      Serial.println(ap.quality);
     }
  );
}

void loop()
{
  wifi.handle();
}
