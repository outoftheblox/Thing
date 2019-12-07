#include "OtaThing.h"

#include <ArduinoOTA.h>

using namespace ootb;

OtaThing::OtaThing()
{
}

OtaThing::~OtaThing()
{
}

void OtaThing::onOtaStart(std::function<void()> f)
{
    otaStartCallback = f;
}

void OtaThing::onOtaEnd(std::function<void()> f)
{
    otaEndCallback = f;
}

void OtaThing::begin()
{
    ArduinoOTA.setPasswordHash("bf3967146d23842e157da467392c4520");
    ArduinoOTA.onStart([]() {
        Serial.println("Start");
        if (otaStartCallback) otaStartCallback();
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
        if (otaStartCallback) otaEndCallback();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
}

void OtaThing::handle()
{
  ArduinoOTA.handle();
}
