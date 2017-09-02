#include "WifiThing.h"

#include <algorithm>
#include <vector>
#include <iterator>

using namespace g3rb3n;

WifiThing::WifiThing()
{
}

WifiThing::~WifiThing()
{
}

void WifiThing::begin()
{
  stateChange("Scanning for networks");
  scan();
  stateChange("Selecting best network");
  sortAccessPointsByQuality();
//  removeUnaccessible();
}

WifiThing::State WifiThing::state() const
{
  return _state;  
}

void WifiThing::setConnectingTimeoutMs(unsigned long ms)
{
  connectingTimeout = ms;
}

void WifiThing::addAccessPoint(String& ssid, String& password)
{
  AccessPoint ap;
  ap.ssid = ssid;
  ap.password = password;
  _accessPoints.push_back(ap);
  stateChange("Added network " + ap.ssid);
}

void WifiThing::addAccessPoint(String& ssid)
{
  AccessPoint ap;
  ap.ssid = ssid;
  _accessPoints.push_back(ap);
  stateChange("Added network " + ap.ssid);
}

void WifiThing::addAccessPoint(char* ssid, char* password)
{
  AccessPoint ap;
  ap.ssid = String(ssid);
  ap.password = String(password);
  _accessPoints.push_back(ap);
  stateChange("Added network " + ap.ssid);
}

std::vector<AccessPoint>& WifiThing::accessPoints()
{
  return _accessPoints;
}

void WifiThing::onStateChange(std::function<void(const String&)> fun)
{
  stateChangeCallback = fun;
}

void WifiThing::stateChange(String& str)
{
  if (this->stateChangeCallback)
    stateChangeCallback(str);
}

void WifiThing::stateChange(const char* chars)
{
  String str(chars);
  if (this->stateChangeCallback)
    stateChangeCallback(str);
}

bool WifiThing::noneAccessible()
{
  return endAccessible() == _accessPoints.begin();
}

void WifiThing::rescan()
{
  stateChange("Scanning for networks");
  scan();
  stateChange("Selecting best network");
  sortAccessPointsByQuality();  
}

void WifiThing::handle()
{
  if (_state != State::NoAccessPoints && noneAccessible())
  {
    stateChange("No known access points in range");
    _state = State::NoAccessPoints;
    return;
  }
  if (_state == State::NoAccessPoints)
  {
    rescan();
    return;
  }
  if (_state == State::Connected && WiFi.status() != WL_CONNECTED)
    _state = State::Disconnected;
  if(_state != State::Connected)
    connect();
}

void WifiThing::connect()
{
  AccessPoint ap = _accessPoints[accessPointIndex];
  if (_state == State::Disconnected)
  {
    _state = State::Connecting;
    stateChange("Connecting to " + ap.ssid);
    WiFi.begin(ap.ssid.c_str(), ap.password.c_str());
    connectingSince = millis();
    return;
  }
  if (_state == State::Connecting && WiFi.status() == WL_CONNECTED)
  {
    _state = State::Connected;
    stateChange("Connected to " + ap.ssid);
    stateChange("Got IP " + WiFi.localIP().toString());
    return;
  }
  if (_state == State::Connecting && millis() > connectingSince + connectingTimeout)
  {
    _state = State::Disconnected;
    stateChange("Time out on connecting to " + ap.ssid);
    ++accessPointIndex;
    if (accessPointIndex >= _accessPoints.size())
      accessPointIndex = 0;
  }
}

void WifiThing::scan()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  std::for_each(
    _accessPoints.begin(), 
    _accessPoints.end(), 
    [](AccessPoint& ap){ ap.quality = -127; }
  );
  numberFoundNetworks = WiFi.scanNetworks();
}

AccessPoint* WifiThing::accessPoint(String& ssid)
{
  std::vector<AccessPoint>::iterator idx = std::find_if(
    _accessPoints.begin(),
    _accessPoints.end(),
    [&ssid](const AccessPoint& ap){ return ap.ssid.equals(ssid); }
  );
  return &(*idx);
}

void WifiThing::addUnsecureAccessPoints()
{
  for (int i = 0; i < numberFoundNetworks; ++i)
  {
    String ssid = WiFi.SSID(i);
    AccessPoint* ap = accessPoint(ssid);
    if (ap == &(*_accessPoints.end()))
      addAccessPoint(ssid);
  }
}

void WifiThing::removeUnaccessible()
{
  auto it = std::remove_if(
    _accessPoints.begin(), _accessPoints.end(), 
    [] (const AccessPoint& ap){
      return ap.quality == -127;
    }
  );
  _accessPoints.erase(it, _accessPoints.end());
}

std::vector<AccessPoint>::iterator WifiThing::endAccessible()
{
  auto it = std::remove_if(
    _accessPoints.begin(), _accessPoints.end(), 
    [] (const AccessPoint& ap){
      return ap.quality == -127;
    }
  );
  return it;
}

void WifiThing::sortAccessPointsByQuality()
{
  for (int i = 0; i < numberFoundNetworks; ++i)
  {
    String ssid = WiFi.SSID(i);
    AccessPoint* ap = accessPoint(ssid);
    if (ap != &(*_accessPoints.end()))
      ap->quality = WiFi.RSSI(i);
  }

  std::sort(
    _accessPoints.begin(), _accessPoints.end(), 
    [] (const AccessPoint& a, const AccessPoint& b){ return a.quality > b.quality; }
  );
}
