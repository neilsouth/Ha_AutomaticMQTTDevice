#include "../include/ha_mqttentity.h"
#include <stdio.h>
#include <cctype>
#include <cstring>

#define entityBufferSize 1500
char buffer[entityBufferSize];
char innerBuffer[200];

ha_mqttentity::ha_mqttentity() {}

bool ha_mqttentity::usesJsonSchema() const
{
  return strstr(extraAttributes, "\"schema\"") != nullptr && strstr(extraAttributes, "\"json\"") != nullptr;
}

void ha_mqttentity::initialiseEntity(const char *locationGiven, const char *name)
{
  strcpy(entityClass, name);
  toLower(entityClassLower, name, 50);
  strcpy(location, locationGiven);
  toLower(locationLower, locationGiven, 50);
  strncpy(shortName, entityClassLower, 4);
  // strncpy(fistLetter,name,1);
  fistLetter[0] = name[0];
  fistLetter[1] = name[1];
  fistLetter[2] = 0;
  inUse = true;
  getStateTopic();
  innerValues[0].setValue(0.0f);
}

const char *ha_mqttentity::getConfig()
{
  //  snprintf(buffer, sizeof(buffer), "{  \"device\": {\"manufacturer\":\"neil\",\"model\":\"multi-sensor\"},\"expire_after\":\"60\" \"device_class\": \"%s\",\"name\": \"%s\", \"state_topic\":\"%s\",\"unit_of_measurement\": \"%s\", \"value_template\": \"{{ value_json.%s}}\", \"unique_id\": \"%s%s\",\"object_id\":\"%s%s\",\"device\":{\"identifiers\": [\"%s%s\"],\"name\": \"%s\"}}",
  //         deviceType, entityClass, stateTopic, unitOfMeasure, entityClassLower, shortName, uidPostfix, shortName, uidPostfix, locationLower, uidPostfix, location);

  int buffSizeRemaining = sizeof(buffer);
  int bufferEnd = (int)buffer + sizeof(buffer);
  int written = 0;
  char *buff = &buffer[0];
  buff[0] = '{';
  buffSizeRemaining--;
  buff++;

  buff += writeDeviceClass(buff, buffSizeRemaining);
  buffSizeRemaining = bufferEnd - (int)buff;

  buff += writeName(buff, buffSizeRemaining);
  buffSizeRemaining = bufferEnd - (int)buff;

  buff += writeStateTopic(buff, buffSizeRemaining);
  buffSizeRemaining = bufferEnd - (int)buff;

  buff += writeCommandTopic(buff, buffSizeRemaining);
  buffSizeRemaining = bufferEnd - (int)buff;

  buff += writeUnits(buff, buffSizeRemaining);
  buffSizeRemaining = bufferEnd - (int)buff;

  buff += writeIds(buff, buffSizeRemaining);
  buffSizeRemaining = bufferEnd - (int)buff;

  buff += writeDeviceJson(buff, buffSizeRemaining);
  buffSizeRemaining = bufferEnd - (int)buff;

  snprintf(buff, buffSizeRemaining, "\"expire_after\":%d %s}",
           expireAfter,
           extraAttributes);
  // Serial.println(buffer);
  return buffer;
}

int ha_mqttentity::writeDeviceClass(char *buffer, int bufferSizeRemaining)
{
  if (strlen(deviceClass) > 1 && strcmp("null", deviceClass) != 0 && strcmp("none", deviceClass) != 0)
  {
    int written = snprintf(buffer, bufferSizeRemaining, "\"device_class\": \"%s\",", deviceClass);
    return written;
  }
  return 0;
}
int ha_mqttentity::writeName(char *buffer, int bufferSizeRemaining)
{
  int written = snprintf(buffer ,bufferSizeRemaining, "\"name\": \"%s\"," , entityClass );
  return written;
}
int ha_mqttentity::writeStateTopic(char* buffer, int bufferSizeRemaining)
{
  int written = snprintf(buffer, bufferSizeRemaining, "\"state_topic\": \"%s\",", stateTopic);
  return written;
}

int ha_mqttentity::writeCommandTopic(char *buffer, int bufferSizeRemaining)
{
  char *topic = getCommandTopic();
  if (strlen(topic) > 1)
  {
    int written = snprintf(buffer, bufferSizeRemaining, "\"command_topic\": \"%s\",", topic);
    return written;
  }
  return 0;
}

int ha_mqttentity::writeUnits(char *buffer, int bufferSizeRemaining)
{
  const char *valueTemplates = getValueTemplates();
  bool hasTemplates = strnlen(valueTemplates, sizeof(innerBuffer)) > 0;

  if (strlen(unitOfMeasure) > 0)
  {
    if (hasTemplates)
    {
      int written = snprintf(buffer, bufferSizeRemaining, "\"unit_of_measurement\": \"%s\", %s,", unitOfMeasure, valueTemplates);
      return written;
    }

    int written = snprintf(buffer, bufferSizeRemaining, "\"unit_of_measurement\": \"%s\",", unitOfMeasure);
    return written;
  }

  if (!hasTemplates)
  {
    return 0;
  }

  int written = snprintf(buffer, bufferSizeRemaining, "%s,", valueTemplates);
  return written;
}
int ha_mqttentity::writeIds(char *buffer, int bufferSizeRemaining)
{
  int written = snprintf(buffer, bufferSizeRemaining, "\"unique_id\": \"%s%s\",",
                         shortName,
                         uidPostfix);
  return written;
}

int ha_mqttentity::writeDeviceJson(char *buffer, int bufferSizeRemaining)
{
  int written = snprintf(buffer, bufferSizeRemaining, "\"device\":{\"identifiers\": [\"%s - %s\"],\"name\": \"%s - %s\",\"mf\": \"%s\" ,\"mdl\": \"%s\"},",
                         locationLower,
                         uidPostfix,
                         location, ip,
                         manufacturer,
                         model);
  return written;
}

bool ha_mqttentity::isInUse()
{
  return inUse;
}

char *ha_mqttentity::getStateTopic()
{
  snprintf(stateTopic, sizeof(stateTopic), "%s/state", locationLower);
  return stateTopic;
}

char *ha_mqttentity::getConfigTopic()
{
  snprintf(configTopic, sizeof(configTopic), "homeassistant/%s/%s%s/config", sensorType, shortName, uidPostfix);
  return configTopic;
}

char *ha_mqttentity::getCommandTopic()
{
  if (!hasCommand)
  {
    commandTopic[0] = 0;
    return commandTopic;
  }

  char topicName[50];
  mqttsubentity::replace_char(entityClassLower, ' ', '_', topicName);
  snprintf(commandTopic, sizeof(commandTopic), "%s/%s/%s/set", locationLower, sensorType, topicName);
  return commandTopic;
}

void ha_mqttentity::setUnitOfMeasure(const char *measure)
{
  strncpy(unitOfMeasure, measure, 4);
}
void ha_mqttentity::setIpAddress(const char* ipAddress)
{
  strncpy(ip, ipAddress, 16);
}

void ha_mqttentity::setValue(float valueGiven, int index)
{
  innerValues[index].setValue(valueGiven);
}
void ha_mqttentity::setValue(bool valueGiven, int index)
{
  innerValues[index].setValue(valueGiven);
}

void ha_mqttentity::setSubName(const char *name, int index)
{
  innerValues[index].setName(name);
}

void ha_mqttentity::setValue(const char *valueGiven, int index)
{
  innerValues[index].setValue(valueGiven);
}
void ha_mqttentity::setValue(int valueGiven, int index)
{
  innerValues[index].setValue(valueGiven);
}
void ha_mqttentity::toLower(char *dest, const char *source, uint16_t destSize)
{
  size_t len = strnlen(source, 200);
  if (strnlen(source, 200) > destSize)
  {
    len = destSize;
  }
  for (size_t i = 0; i < len; i++)
  {
    dest[i] = std::tolower(source[i]);
  }
  if (destSize > strnlen(source, 200))
  {
    for (size_t i = len; i < destSize; i++)
    {
      dest[i] = 0;
    }
  }
}

const char *ha_mqttentity::getJsonValueString()
{
  int innerCount = innerValuesCount();
  char* newName = mqttsubentity::replace_char(entityClassLower, ' ', '_', buffer2);

  if (usesJsonSchema())
  {
    char *buf = buffer;
    size_t remaining = sizeof(buffer);

    for (int i = 0; i < innerCount; ++i)
    {
      const char *innerValue = innerValues[i].getJson();
      size_t len = strnlen(innerValue, 200);
      snprintf(buf, remaining, "%s%s", innerValue, (i < innerCount - 1) ? "," : "");
      size_t written = len + ((i < innerCount - 1) ? 1 : 0);
      buf += written;
      remaining -= written;
    }

    return buffer;
  }

  if (innerCount == 1)
  {
    snprintf(buffer, sizeof(buffer), "\"%s\" :%s", newName, innerValues[0].getJsonValue());
    return buffer;
  }

  char *buf = buffer;
  size_t remaining = sizeof(buffer);

  snprintf(buf, remaining, "\"%s\": { ", newName);
  size_t len = strnlen(newName, 50);
  remaining -= len + 6;
  buf += len + 6;

  for (int i = 0; i < innerCount - 1; ++i)
  {
    const char *innerValue = innerValues[i].getJson();
    size_t len = strnlen(innerValue, 200);
    snprintf(buf, remaining, "%s,", innerValue);

    buf += len + 1;
    remaining -= len + 1;
  }
  snprintf(buf, remaining, "%s}", innerValues[innerCount - 1].getJson());

  return buffer;
}

const char *ha_mqttentity::name()
{
  return entityClass;
}

void ha_mqttentity::setDeviceClass(const char *type)
{
  toLower(deviceClass, type, 50);
}

void ha_mqttentity::setSensorType(const char *type)
{
  toLower(sensorType, type, 50);
  hasCommand = strcmp(sensorType, "switch") == 0 ||
               strcmp(sensorType, "light") == 0 ||
               strcmp(sensorType, "text") == 0;
  if (hasCommand)
  {
    getCommandTopic();
  }
}

void ha_mqttentity::setExpireAfter(int value)
{
  expireAfter = value;
}

void ha_mqttentity::setUid(const char *uid)
{
  strncpy(uidPostfix, uid, 5);
  strcat(uidPostfix, ""); // append zero terminator
}

void ha_mqttentity::setExtraAttibutes(const char *extra)
{
  strncpy(extraAttributes, ", ", 3);
  strncpy(&extraAttributes[2], extra, 254);
}

int ha_mqttentity::innerValuesCount()
{
  for (int i = 0; i < 4; ++i)
  {
    if (innerValues[i].inUse == false)
    {
      return i;
    }
  }
  return 4;
}

void ha_mqttentity::setSubValueTemplate(const char *name, int index)
{
  innerValues[index].setValueTemplate(name);
}

char *ha_mqttentity::getValueTemplates()
{
  innerBuffer[0] = 0;

  if (usesJsonSchema())
  {
    return innerBuffer;
  }

  char* buf = innerBuffer;
  size_t sizeBuf = sizeof(innerBuffer);
  for (int i = 0; i < 4; ++i)
  {
    if(innerValues[i].inUse)
    {
      if(i != 0)
      {
        buf[0] = ',';
        buf++;
        sizeBuf--;
      }
      char* valTemp = innerValues[i].getValueTemplate( entityClassLower );
      strncpy(buf, valTemp , sizeBuf);
      size_t resultLen = strnlen(valTemp , 200);
      buf+= resultLen;
      sizeBuf -= resultLen;
    }
  }
  return innerBuffer;
}