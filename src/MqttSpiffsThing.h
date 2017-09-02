#ifndef _MqttSpiffsThing_H
#define _MqttSpiffsThing_H

#include "MqttThing.h"

namespace g3rb3n
{

  class MqttSpiffsThing : public MqttThing
  {
    private:
      String filename;

    public:
      MqttSpiffsThing(const char* name);
      MqttSpiffsThing(String& filename);
      ~MqttSpiffsThing();

      void begin();
      
    private:

  };
}

#endif
