#ifndef _SpiffsTextSettings_H
#define _SpiffsTextSettings_H

#include <FS.h>

namespace ootb
{

    class SpiffsTextSettings
    {
    private:
        File configFile;

    public:
        SpiffsTextSettings();

        bool begin();

        bool load(const char *filename);

        String readString();
        int readInt();
        bool readBool();
    };
}

#endif
