#include "WifiSpiffsThing.h"
#include "SpiffsTextSettings.h"

using namespace ootb;

WifiSpiffsThing::WifiSpiffsThing(String& name)
:
filename(name)
{
}

WifiSpiffsThing::WifiSpiffsThing(const char* name)
:
filename(String(name))
{
}

WifiSpiffsThing::~WifiSpiffsThing()
{
}

void WifiSpiffsThing::begin()
{
  SpiffsTextSettings settings;
  settings.begin();
  bool success = settings.load(filename.c_str());
  if (!success)
  {
    Serial.println("Failed to load config");
  }
  if (success)
  {
    while(true)
    {
      String ssid = settings.readString();
      if (ssid.length() == 0)
        break;
      String password = settings.readString();
      addAccessPoint(ssid, password);
    }
  }
  WifiThing::begin();
}
