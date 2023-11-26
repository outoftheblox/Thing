#include "OtaSpiffsThing.h"
#include "SpiffsTextSettings.h"

using namespace ootb;

OtaSpiffsThing::OtaSpiffsThing(String &name)
    : filename(name)
{
}

OtaSpiffsThing::OtaSpiffsThing(const char *name)
    : filename(String(name))
{
}

OtaSpiffsThing::~OtaSpiffsThing()
{
}

void OtaSpiffsThing::begin()
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
        String password = settings.readString();
        if (password.length() == 0)
            Serial.println("Failed to load password");
        setPassword(password.c_str());
    }
    OtaThing::begin();
}
