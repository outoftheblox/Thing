#include "MqttWifiSpiffsThing.h"

using namespace ootb;

MqttWifiSpiffsThing::MqttWifiSpiffsThing()
:
WifiSpiffsThing("/wifi.txt"),
MqttSpiffsThing("/mqtt.txt")
{
}

MqttWifiSpiffsThing::~MqttWifiSpiffsThing()
{
}

void MqttWifiSpiffsThing::begin()
{
  WifiSpiffsThing::begin();
  MqttSpiffsThing::begin();
}

void MqttWifiSpiffsThing::handle()
{
  WifiSpiffsThing::handle();
  if (WifiSpiffsThing::state() == WifiThing::State::Connected)
    MqttSpiffsThing::handle();
}

void MqttWifiSpiffsThing::onStateChange(std::function<void(const String&)> callback)
{
  WifiSpiffsThing::onStateChange(callback);
  MqttSpiffsThing::onStateChange(callback);  
}
