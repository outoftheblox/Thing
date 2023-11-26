#ifndef _MqttWifiSpiffsOtaThing_H
#define _MqttWifiSpiffsOtaThing_H

#include <MqttWifiSpiffsThing.h>
#include <OtaSpiffsThing.h>

namespace ootb
{

    class MqttWifiSpiffsOtaThing : public MqttWifiSpiffsThing, public OtaSpiffsThing
    {
    public:
        MqttWifiSpiffsOtaThing();
        ~MqttWifiSpiffsOtaThing();

        void begin();
        void handle();
    };
}

#endif
