#include "MqttSpiffsThing.h"

#include <SpiffsTextSettings.h>

using namespace ootb;

MqttSpiffsThing::MqttSpiffsThing(String &name)
    : filename(name)
{
    Serial.println("Init");
}

MqttSpiffsThing::MqttSpiffsThing(const char *name)
    : filename(String(name))
{
    Serial.println("Init");
}

MqttSpiffsThing::~MqttSpiffsThing()
{
}

void MqttSpiffsThing::begin()
{
    Serial.println("Begin");
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
        bool useTLS = settings.readBool();
        setServer(server, port, useTLS);
        setCredentials(username, password);
    }
    MqttThing::begin();
}
