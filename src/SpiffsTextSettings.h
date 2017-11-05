#ifndef _SpiffsTextSettings_H
#define _SpiffsTextSettings_H

#include <FS.h>

namespace g3rb3n
{

  class SpiffsTextSettings
  {
    private:
      File configFile;

    public:
      SpiffsTextSettings();

      bool begin();

      bool load(const char* filename);

      String readString();
      int readInt();
      bool readBool();
  };
}

#endif
