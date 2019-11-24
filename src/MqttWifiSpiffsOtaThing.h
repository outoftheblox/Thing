#ifndef _MqttWifiSpiffsOtaThing_H
#define _MqttWifiSpiffsOtaThing_H

#include <MqttWifiSpiffsThing.h>
#include <OtaThing.h>

namespace ootb
{

  class MqttWifiSpiffsOtaThing : public MqttWifiSpiffsThing, public OtaThing
  {
    public:
      MqttWifiSpiffsOtaThing();
      ~MqttWifiSpiffsOtaThing();

      void begin();
      void handle();
  };
}

#endif
