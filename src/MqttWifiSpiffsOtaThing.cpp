#include "MqttWifiSpiffsOtaThing.h"

using namespace ootb;

MqttWifiSpiffsOtaThing::MqttWifiSpiffsOtaThing()
:
    OtaSpiffsThing("/ota.txt")
{
}

MqttWifiSpiffsOtaThing::~MqttWifiSpiffsOtaThing()
{
}

void MqttWifiSpiffsOtaThing::begin()
{
    MqttWifiSpiffsThing::begin();
    OtaSpiffsThing::begin();
}

void MqttWifiSpiffsOtaThing::handle()
{
    MqttWifiSpiffsThing::handle();
    OtaSpiffsThing::handle();
}
