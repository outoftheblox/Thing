#ifndef _OtaThing_H
#define _OtaThing_H

namespace ootb
{

class OtaThing
{
    public:
    OtaThing();
    ~OtaThing();

    void begin();
    void handle();

    void onOtaStart(std::function<void()> f);
    void onOtaEnd(std::function<void()> f);

    private:
    std::function<void()> otaStartCallback;
    std::function<void()> otaEndCallback;
};
}

#endif
