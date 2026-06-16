// entity.h
#ifndef H_MQTTENTITY_h
#define H_MQTTENTITY_h

//#include <Arduino.h>
#include <cstdint>
#include "ha_mqttsubentity.h"

class ha_mqttentity {
private:

  char buffer2[50];
  char entityClass[50];       //"Temperature"
  char entityClassLower[50];  //"temperature"
  char shortName[5];          //"temp"
  char fistLetter[5];         // "T"
  char location[50];          //"Bathroom";
  char locationLower[50];     //"bathroom";
  char unitOfMeasure[8] = "";
  char uidPostfix[6]="01ae";
  char stateTopic[128] ="";
  char sensorType[128] ="sensor";
  char deviceClass[50] = "";
  char configTopic[128] ="";
  char commandTopic[128] = "";
  char stringValue[128] ="";
  char valueStr[20] ="";
  char manufacturer[20] = "Neil South";
  char model[25] = "South ESP32 v1.0";
  char ip[18] = "0.0.0.0";
  char extraAttributes[256] = "";
  //float value;
  void toLower( char* dest, const char* source , uint16_t destSize);
  bool inUse = false;
  bool hasCommand = false;
  int expireAfter = 60;

  mqttsubentity innerValues[4];

  int innerValuesCount();
public:

  ha_mqttentity();
  void initialiseEntity(const char* locationGiven, const char* name);
  const char* getConfig();
  bool isInUse();
  void setUnitOfMeasure(const char* measure);
  char* getStateTopic();
  char* getConfigTopic();
  char* getCommandTopic();
  char* getValueTemplates();
  void setValue(float value, int index =0);
  void setValue(const char* value, int index = 0);
  void setValue(bool value, int index = 0);
  void setValue(int value, int index = 0);
  void setSubName(const char* name, int index = 0);
  void setSubValueTemplate(const char* name, int index = 0);
  void setDeviceClass(const char* type);
  const char* getJsonValueString();
  const char* name();
  void setExpireAfter(int value);
  void setSensorType(const char* type);
  void setIpAddress(const char* ip);
  void setUid(const char* uid);
  void setExtraAttibutes(const char* extra);

  int writeDeviceClass(char* buffer, int bufferSizeRemaining);
  int writeName(char* buffer, int bufferSizeRemaining);
  int writeStateTopic(char* buffer, int bufferSizeRemaining);
  int writeCommandTopic(char* buffer, int bufferSizeRemaining);
  int writeUnits(char* buffer, int bufferSizeRemaining);
  int writeIds(char* buffer, int bufferSizeRemaining);
  int writeDeviceJson(char *buffer, int bufferSizeRemaining);
};

#endif