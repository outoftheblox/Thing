#ifndef _OtaSpiffsThing_H
#define _OtaSpiffsThing_H

#include "OtaThing.h"

namespace ootb
{

    class OtaSpiffsThing : public OtaThing
    {
    private:
        String filename;

    public:
        OtaSpiffsThing(const char *name);
        OtaSpiffsThing(String &filename);
        ~OtaSpiffsThing();

        void begin();

    private:
    };
}

#endif
