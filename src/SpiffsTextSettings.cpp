#include "SpiffsTextSettings.h"

namespace ootb
{

    SpiffsTextSettings::SpiffsTextSettings()
    {
    }

    bool SpiffsTextSettings::begin()
    {
        Serial.println("Mounting FS...");

        if (!SPIFFS.begin())
        {
            Serial.println("Failed to mount file system");
            return false;
        }
        return true;
    }

    bool SpiffsTextSettings::load(const char *filename)
    {
        configFile = SPIFFS.open(filename, "r");
        if (!configFile)
        {
            Serial.println("Failed to open config file");
            return false;
        }

        size_t size = configFile.size();
        if (size > 1024)
        {
            Serial.println("Config file size is too large");
            return false;
        }

        return true;
    }

    String SpiffsTextSettings::readString()
    {
        char buffer[100];
        int read = configFile.readBytesUntil('\n', buffer, 100);
        buffer[read] = 0;
        return String(buffer);
    }

    int SpiffsTextSettings::readInt()
    {
        char buffer[100];
        int read = configFile.readBytesUntil('\n', buffer, 100);
        buffer[read] = 0;
        return atoi(buffer);
    }

    bool SpiffsTextSettings::readBool()
    {
        char buffer[100];
        int read = configFile.readBytesUntil('\n', buffer, 100);
        buffer[read] = 0;
        if (buffer[0] == 't')
            return true;
        if (buffer[0] == 'f')
            return false;
        return atoi(buffer);
    }
}

/*
bool saveConfig() {
StaticJsonBuffer<200> jsonBuffer;
JsonObject& json = jsonBuffer.createObject();
json["serverName"] = "api.example.com";
json["accessToken"] = "128du9as8du12eoue8da98h123ueh9h98";

File configFile = SPIFFS.open("/config.json", "w");
if (!configFile) {
  Serial.println("Failed to open config file for writing");
  return false;
}

json.printTo(configFile);
return true;
}
*/
