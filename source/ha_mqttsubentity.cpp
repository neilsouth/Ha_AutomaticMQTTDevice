#include "../include/ha_mqttsubentity.h"
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <cmath>

char* mqttsubentity::toString()
{
	snprintf(buffer, sizeof(buffer), "");
	return buffer;
}

char* mqttsubentity::getJsonName()
{
	snprintf(buffer, sizeof(buffer), "\"%s\"", entityName);
	return buffer;
}

char* mqttsubentity::getJsonValue()
{
	buffer2[0] = 0;
	switch (type)
	{
	case mqttsubentity::typeEnum::boolT:
		if (valueB)
			snprintf(buffer2, sizeof(buffer2), "\"ON\"");
		else
			snprintf(buffer2, sizeof(buffer2), "\"OFF\"");
		break;
	case mqttsubentity::typeEnum::stringT:
		snprintf(buffer2, sizeof(buffer2), "\"%s\"", valueS);
		break;
	case mqttsubentity::typeEnum::floatT:
	{
		char float_str[10];
		snprintf(float_str, sizeof(float_str), "%.2f", valueF);
		snprintf(buffer2, sizeof(buffer2), "%s", float_str);
		break;
	}
	case mqttsubentity::typeEnum::intT:
	{
		snprintf(buffer2, sizeof(buffer2), "%d", valueI);
		break;
	}
	default:
		break;
	}
	return buffer2;
}

char* mqttsubentity::getJson()
{
	char* nameStr = getJsonName();
	char* valStr = getJsonValue();
	size_t size = strnlen(nameStr, sizeof(buffer));
	snprintf(buffer + size, sizeof(buffer) - size, ": %s", valStr);
	return buffer;
}

void mqttsubentity::setValue(const char* value)
{
	inUse = true;
	type = typeEnum::stringT;
	strncpy(valueS, value, sizeof(valueS));
}
void mqttsubentity::setValue(float value)
{
	inUse = true;
	type = typeEnum::floatT;
	valueF = value;
}
void mqttsubentity::setValue(bool value)
{
	inUse = true;
	type = typeEnum::boolT;
	valueB = value;
}
void mqttsubentity::setValue(int value)
{
	inUse = true;
	type = typeEnum::intT;
	valueI = value;
}
void mqttsubentity::setValueTemplate(const char* templateName)
{
	strncpy(valueTemplate, templateName, sizeof(valueTemplate));
}
void mqttsubentity::setName(const char* value)
{
	strncpy(entityName, value, sizeof(entityName));
}
char* mqttsubentity::getValueTemplate(const char *parentEntityName)
{

	char* newName = replace_char(parentEntityName, ' ', '_', buffer2);

	if (strcmp(parentEntityName, "light") == 0)
	{
		if (strcmp(entityName, "state") == 0)
		{
			snprintf(buffer, sizeof(buffer), "\"state_value_template\": \"{{ value_json.light.state }}\"");
		}
		else if (strcmp(entityName, "brightness") == 0)
		{
			snprintf(buffer, sizeof(buffer), "\"brightness_value_template\": \"{{ value_json.light.brightness }}\"");
		}
	}
	else
	{
		if (strncmp("none", entityName, 200) == 0)
			snprintf(buffer, sizeof(buffer), "\"%s\": \"{{ value_json.%s}}\"", valueTemplate, newName);
		else
			snprintf(buffer, sizeof(buffer), "\"%s\": \"{{ value_json.%s.%s}}\"", valueTemplate, newName, entityName);

		return buffer;
	}
	return buffer;
}

