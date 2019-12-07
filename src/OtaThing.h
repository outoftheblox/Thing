#ifndef _OtaThing_H
#define _OtaThing_H

#include <Arduino.h>
#include <functional>

namespace ootb
{

class OtaThing
{
    public:
    OtaThing();
    OtaThing(const char* password);
    ~OtaThing();

    void setPassword(String _password);

    void begin();
    void handle();

    void onOtaStart(std::function<void()> f);
    void onOtaEnd(std::function<void()> f);

    private:
    std::function<void()> otaStartCallback;
    std::function<void()> otaEndCallback;
};

    private:
    String password;
    std::function<void()> otaStartCallback;
    std::function<void()> otaEndCallback;
};

}

#endif
