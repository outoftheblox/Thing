#include "MqttWifiSpiffsOtaThing.h"

using namespace ootb;

MqttWifiSpiffsOtaThing::MqttWifiSpiffsOtaThing()
{
}

MqttWifiSpiffsOtaThing::~MqttWifiSpiffsOtaThing()
{
}

void MqttWifiSpiffsOtaThing::begin()
{
  MqttWifiSpiffsThing::begin();
  OtaThing::begin();
}

void MqttWifiSpiffsOtaThing::handle()
{
  MqttWifiSpiffsThing::handle();
  OtaThing::handle();
}
