// ha_mqttdevice.h
#ifndef H_MQTTDEVICE_h
#define H_MQTTDEVICE

#include <Arduino.h>
#include "ha_mqttentity.h"

#ifndef HA_MQTTDEVICE_LOG
#define HA_MQTTDEVICE_LOG(message) ((void)0)
#endif

#ifndef HA_MQTTDEVICE_LOGLN
#define HA_MQTTDEVICE_LOGLN(message) ((void)0)
#endif

#define NumOfEntities 10
#define deviceBufferSize 5000 //entityBufferSize * NumOfEntities

class ha_mqttdevice {
private:
  char buffer[deviceBufferSize+1];

  const char* DeviceName;
  ha_mqttentity entitys[NumOfEntities] = {};
  int index =0;
  ha_mqttentity* getNextEntity();

public:
  ha_mqttdevice(const char* Name);
  ha_mqttentity* addEntity( const char* name);
  const char* getConfig();
  const char* getStateTopic();
  template <typename TMqttClient>
  void registerDevice(TMqttClient* client)
  {
    for (int i = 0; i < NumOfEntities; ++i)
    {
      if (entitys[i].isInUse() == true)
      {
        const char* config = entitys[i].getConfig();
        const char* topic = entitys[i].getConfigTopic();
        HA_MQTTDEVICE_LOG("registering entity topic: ");
        HA_MQTTDEVICE_LOGLN(topic);
        HA_MQTTDEVICE_LOGLN(config);
        client->publish(topic, config, true);
      }
    }
  }

  template <typename TMqttClient>
  void publishState(TMqttClient* client)
  {
    char* state = getPublishState();
    size_t size = strnlen(state, deviceBufferSize);
    if (size > 2)
    {
      client->publish(getStateTopic(), getPublishState());
    }
  }
  char* getPublishState();
  void setUid(const char* uid);
  void setIpAddress(const char* ipAddress);
};

#endif