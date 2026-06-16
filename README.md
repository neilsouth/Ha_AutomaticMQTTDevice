# Ha_AutoMQTTDevice

Standalone git repository for the `Ha_AutoMQTTDevice` library, seeded from the latest copy found in `Esp32GreenHouse` and updated with the newer `ha_mqttsubentity` files from `PatioFairyLightsESP32`.

## Structure

- `include/` public headers
- `source/` implementation files
- `library.json` valid PlatformIO library manifest
- `libaray.json` original source manifest kept as copied from the project

## Notes

This repo no longer requires project-local `debugLogs.h` or `EspMQTTClient.h` headers.

Any MQTT client type with compatible `publish(topic, payload, retain)` and `publish(topic, payload)` methods can be passed to `registerDevice(...)` and `publishState(...)`.

Optional logging can be enabled by defining `HA_MQTTDEVICE_LOG(...)` and `HA_MQTTDEVICE_LOGLN(...)` before including `ha_mqttdevice.h`.