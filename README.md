# Ha_AutoMQTTDevice

[![Version](https://img.shields.io/badge/version-1.0.0-2ea44f.svg)](https://github.com/neilsouth/Ha_AutomaticMQTTDevice/releases)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-compatible-orange.svg)](https://platformio.org/)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

Small Arduino/ESP helper library for publishing Home Assistant MQTT discovery payloads and device state JSON.

The library is aimed at microcontroller projects that already have an MQTT client and want a lightweight way to:

- describe one device with multiple entities
- generate Home Assistant discovery config payloads
- publish JSON state payloads in a consistent format
- support sensors, switches, lights, and simple multi-value entities

## Features

- Home Assistant MQTT discovery payload generation
- Multiple entities per device
- Multiple sub-values per entity
- Simple light/switch command topic support
- Works with any MQTT client exposing compatible `publish(...)` methods
- Optional logging hooks via macros

## Installation

### PlatformIO

Add this to `lib_deps`:

```ini
lib_deps =
	https://github.com/neilsouth/Ha_AutomaticMQTTDevice.git
```

If you also need an MQTT connection manager for ESP8266 or ESP32 projects, the companion [`EspMQTTClient`](https://github.com/neilsouth/EspMQTTClient) library is a good fit.

### Local library

Clone or copy this repository into your project's `lib/Ha_AutoMQTTDevice` folder.

## Compatibility

- Designed for Arduino-style embedded projects
- Tested in a PlatformIO workflow
- Works with MQTT clients that provide compatible `publish(topic, payload)` and `publish(topic, payload, retain)` methods
- Common fit: ESP8266 and ESP32 projects using an external MQTT connection manager
- Recommended companion client: [`EspMQTTClient`](https://github.com/neilsouth/EspMQTTClient)

## Quick Start

```cpp
#include <Arduino.h>
#include "EspMQTTClient.h"
#include "ha_mqttdevice.h"

EspMQTTClient client(
	"your-ssid",
	"your-password",
	"mqtt-broker.local",
	"mqtt-user",
	"mqtt-password",
	"demo-client"
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
	temperature->setUid("temp1");
}

void loop()
{
	client.loop();

	temperature->setValue(21.5f);
	device.publishState(&client);

	delay(5000);
}
```

## Examples

- `examples/BasicSensor/BasicSensor.ino`: single temperature sensor with discovery and state publishing
- `examples/BasicLight/BasicLight.ino`: light entity with `state` and `brightness` sub-values

## Common API

### Device

```cpp
ha_mqttdevice device("room-name");
ha_mqttentity* entity = device.addEntity("Temperature");
device.registerDevice(&client);
device.publishState(&client);
```

### Entity configuration

```cpp
entity->setDeviceClass("temperature");
entity->setSensorType("sensor");
entity->setUnitOfMeasure("C");
entity->setUid("temp1");
entity->setIpAddress("192.168.1.20");
entity->setExpireAfter(60);
```

### Entity values

```cpp
entity->setValue(22.4f);
entity->setValue("ON");
entity->setValue(42);
entity->setValue(true);
```

### Multi-value entities

Use indexed sub-values when you want a JSON object instead of a single scalar value.

```cpp
ha_mqttentity* light = device.addEntity("light");
light->setSensorType("light");

light->setSubName("state", 0);
light->setSubValueTemplate("state_value_template", 0);
light->setValue("ON", 0);

light->setSubName("brightness", 1);
light->setSubValueTemplate("brightness_value_template", 1);
light->setBrightness(180);
```

This produces state payloads in the shape:

```json
{
	"light": {
		"state": "ON",
		"brightness": 180
	}
}
```

## Topic Format

The library generates Home Assistant discovery topics and device state topics for you.

Typical discovery topics look like:

```text
homeassistant/sensor/temp1234/config
homeassistant/light/kitchen/config
```

Typical state topics look like:

```text
office/state
homeassistant/light/Kitchen/state
```

The exact topic depends on the configured sensor type and entity naming.

For single-value entities, state payloads are simple JSON values wrapped in a device object:

```json
{
	"temperature": 21.50
}
```

For multi-value entities, payloads become nested JSON objects:

```json
{
	"light": {
		"state": "ON",
		"brightness": 180
	}
}
```

## Logging

Logging is optional. Define these macros before including `ha_mqttdevice.h` if you want to see discovery publishes during development:

```cpp
#define HA_MQTTDEVICE_LOG(x) Serial.print(x)
#define HA_MQTTDEVICE_LOGLN(x) Serial.println(x)
#include "ha_mqttdevice.h"
```

## Notes

- This library focuses on MQTT discovery payload generation and state publishing, not network management.
- You are expected to provide Wi-Fi and MQTT connection handling in your application.
- The API is intentionally small and currently optimized for embedded projects rather than desktop testing.

## License

MIT