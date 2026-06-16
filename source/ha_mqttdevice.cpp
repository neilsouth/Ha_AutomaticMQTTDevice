#include "../include/ha_mqttdevice.h"
#include <cstring>

ha_mqttdevice::ha_mqttdevice(const char *name)
{
    DeviceName = name;
}

ha_mqttentity *ha_mqttdevice::addEntity(const char *name)
{
    ha_mqttentity *nextEntity = getNextEntity();
    if (nextEntity == nullptr)
    {
        // log("Error no Enitities left");
        return nullptr;
    }
    nextEntity->initialiseEntity(DeviceName, name);
    return nextEntity;
}

ha_mqttentity *ha_mqttdevice::getNextEntity()
{
    for (int i = 0; i < NumOfEntities; ++i)
    {
        if (entitys[i].isInUse() == false)
        {
            return &entitys[i];
        }
    }
    return nullptr;
}
const char *ha_mqttdevice::getConfig()
{
    uint16_t spaceLeft = deviceBufferSize;
    char *bufferPosition = buffer;
    for (int i = 0; i < NumOfEntities; ++i)
    {
        if (entitys[i].isInUse() == true)
        {
            const char *entry = entitys[i].getConfig();
            uint16_t length = strnlen(entry, 1500);
            strncpy(bufferPosition, entry, spaceLeft);
            spaceLeft -= length;
            bufferPosition += length;
        }
    }
    return buffer;
}
const char *ha_mqttdevice::getStateTopic()
{
    return entitys[0].getStateTopic();
}
char *ha_mqttdevice::getPublishState()
{
    char *bufferPos = buffer;
    char *bufferEnd = buffer + deviceBufferSize;
    bool first = true;
    strncpy(bufferPos++, "{ ", 2);
    for (int i = 0; i < NumOfEntities; ++i)
    {
        if (entitys[i].isInUse() == true)
        {
            if (first == false)
            {
                strncpy(bufferPos, ", ", 2);
                bufferPos += 2;
            }
            const char *json = entitys[i].getJsonValueString();
            uint16_t length = strnlen(json, 200);
            strncpy(bufferPos, json, bufferEnd - bufferPos);
            bufferPos += length;
            first = false;
        }
    }
    strncpy(bufferPos, " } ", 3);
    if (strnlen(buffer, deviceBufferSize) > 2)
    {
        return buffer;
        // logln("published");
        // logln(buffer);
    }
    else
    {
        // logln("No entites found!");
    }
    return buffer;
}

void ha_mqttdevice::setUid(const char *uid)
{
    for (int i = 0; i < NumOfEntities; ++i)
    {
        if (entitys[i].isInUse() == true)
        {
            entitys[i].setUid(uid);
        }
    }
}

void ha_mqttdevice::setIpAddress(const char* ipAddress)
{
    for (int i = 0; i < NumOfEntities; ++i)
    {
        if (entitys[i].isInUse() == true)
        {
            entitys[i].setIpAddress(ipAddress);
        }
    }
}