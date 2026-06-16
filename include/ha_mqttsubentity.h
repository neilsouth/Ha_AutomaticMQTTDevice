// mqqtsubentity.h
#ifndef H_MQTTSUBENTITY_h
#define H_MQTTSUBENTITY_h

#include <cstring>

class mqttsubentity
{
	
	char entityName[50] = "none";
	char valueS[30] = "not set";
	char valueTemplate[40] = "value_template";
	float valueF;
	int valueI;
	bool valueB;
	enum class typeEnum
	{
		boolT,
		stringT,
		floatT,
		intT
	};
	char buffer[200] = { 0 };
	char buffer2[200] = { 0 };;

	typeEnum type;

public:
	bool inUse = false;
	void setName(const char* name);
	void setValueTemplate(const char* templateName);
	void setValue(float value);
	void setValue(const char* value);
	void setValue(bool value);
	void setValue(int value);
	float getValueF()
	{
		return valueF;
	}
	int getValueI()
	{
		return valueI;
	}
	bool getValueB()
	{
		return valueB;
	}
	char *getValueS()
	{
		return valueS;
	}
	char *toString();
	char *getJsonName();
	char *getJsonValue();
	char *getJson();
	char *getValueTemplate(const char *parentEntityName);

	static char * replace_char(const char *input, char find, char replace, char *buffer2)
	{

		for (int i = 0; i < strlen(input); i++)
		{
			if (input[i] == find)
				buffer2[i] = replace;
			else
				buffer2[i] = input[i];
		}

		buffer2[strlen(input)] = '\0';

		return buffer2;
	}
};

#endif