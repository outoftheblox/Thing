#ifndef _WifiThing_H
#define _WifiThing_H

#include <vector>
#include <ESP8266WiFi.h>

namespace ootb
{
  struct AccessPoint
  {
    String ssid;
    String password;
    int8_t quality = -127;
  };

  class WifiThing
  {
    public:
      enum class State
      {
        NoAccessPoints,
        Disconnected,
        Connecting,
        Connected
      };

    private:
      State _state = State::Disconnected;

      unsigned long connectingSince = 0;
      unsigned long connectingTimeout = 10000;

      unsigned int numberFoundNetworks = 0;

      std::vector<AccessPoint> _accessPoints;
      uint8_t accessPointIndex = 0;

      std::function<void(const String&)> stateChangeCallback;

    public:
      WifiThing();
      ~WifiThing();

      void begin();
      void handle();

      State state() const;
      bool connected() const;

      void addAccessPoint(char* ssid, char* password);
      void addAccessPoint(String& ssid, String& pwd);
      void addAccessPoint(String& ssid);

      std::vector<AccessPoint>& accessPoints();

      void onStateChange(std::function<void(const String&)> f);
      void setConnectingTimeoutMs(unsigned long ms);

    private:
      void stateChange(const char* chars);
      void stateChange(String& str);

      std::vector<AccessPoint>::iterator endAccessible();
      void sortAccessPointsByQuality();
      void removeUnaccessible();
      bool noneAccessible();
      void rescan();
      AccessPoint* accessPoint(String& ssid);
      void addUnsecureAccessPoints();
      void scan();
      void connect();

  };
}

#endif
