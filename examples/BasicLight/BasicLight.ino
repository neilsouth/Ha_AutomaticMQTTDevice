#include <Arduino.h>
#include "EspMQTTClient.h"
#include "ha_mqttdevice.h"

EspMQTTClient client(
  "your-ssid",
  "your-password",
  "mqtt-broker.local",
  "mqtt-user",
  "mqtt-password",
  "basic-light"
);

ha_mqttdevice device("living-room");
ha_mqttentity* light = nullptr;

void onConnectionEstablished()
{
  device.registerDevice(&client);
}

void setup()
{
  light = device.addEntity("light");
  light->setSensorType("light");
  light->setUid("l001");
  light->setIpAddress("192.168.1.30");

  light->setSubName("state", 0);
  light->setSubValueTemplate("state_value_template", 0);
  light->setValue("ON", 0);

  light->setSubName("brightness", 1);
  light->setSubValueTemplate("brightness_value_template", 1);
  light->setBrightness(180);
}

void loop()
{
  client.loop();
  device.publishState(&client);
  delay(5000);
}