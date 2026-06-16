#include <Arduino.h>
#include "EspMQTTClient.h"
#include "ha_mqttdevice.h"

EspMQTTClient client(
  "your-ssid",
  "your-password",
  "mqtt-broker.local",
  "mqtt-user",
  "mqtt-password",
  "basic-sensor"
);

ha_mqttdevice device("office");
ha_mqttentity* temperature = nullptr;

void onConnectionEstablished()
{
  device.registerDevice(&client);
}

void setup()
{
  temperature = device.addEntity("Temperature");
  temperature->setDeviceClass("temperature");
  temperature->setUnitOfMeasure("C");
  temperature->setUid("t001");
  temperature->setIpAddress("192.168.1.20");
}

void loop()
{
  client.loop();

  float demoTemperature = 21.5f;
  temperature->setValue(demoTemperature);
  device.publishState(&client);

  delay(5000);
}