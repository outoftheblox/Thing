#ifndef _WifiSpiffsThing_H
#define _WifiSpiffsThing_H

#include "WifiThing.h"

namespace g3rb3n
{

  class WifiSpiffsThing : public WifiThing
  {
    private:
      String filename;

    public:
      WifiSpiffsThing(const char* name);
      WifiSpiffsThing(String& filename);
      ~WifiSpiffsThing();

      void begin();
      
    private:

  };
}

#endif
