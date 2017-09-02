#include "MqttSpiffsThing.h"

#include <SpiffsTextSettings.h>

using namespace g3rb3n;

MqttSpiffsThing::MqttSpiffsThing(String& name)
:
filename(name)
{
}

MqttSpiffsThing::MqttSpiffsThing(const char* name)
:
filename(String(name))
{
}

MqttSpiffsThing::~MqttSpiffsThing()
{
}

void MqttSpiffsThing::begin()
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
    String server = settings.readString();
    uint16_t port = settings.readInt();
    String username = settings.readString();
    String password = settings.readString();
    setServer(server, port);
    setCredentials(username, password);
  }
  MqttThing::begin();
}
