#ifndef _MqttWifiSpiffsThing_H
#define _MqttWifiSpiffsThing_H

#include <MqttSpiffsThing.h>
#include <WifiSpiffsThing.h>

namespace ootb
{

    class MqttWifiSpiffsThing : public MqttSpiffsThing, public WifiSpiffsThing
    {
    public:
        MqttWifiSpiffsThing();
        ~MqttWifiSpiffsThing();

        void begin();
        void handle();
        void onStateChange(std::function<void(const String &)> callback);
    };
}

#endif
