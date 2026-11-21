#include <ThingerESP8266.h>
#define THINGER_OTA_VERSION 1.1.0
#include <ThingerESP8266OTA.h>
#ifdef PLATFORMIO
#include "bms_runtime_secrets.h"
#else
#include "arduino_secrets.h"
#endif
#include "telemetry.h"

ThingerESP8266 thing(THINGER_USERNAME, THINGER_DEVICE_ID, THINGER_DEVICE_CREDENTIAL);
ThingerESP8266OTA ota(thing);
static BmsSample sample;
static bool hasSample;
static uint32_t lastSample;
static uint32_t validFrames;
static uint32_t rejectedFrames;
#include "protection.h"

static void readTelemetry() {
    static char line[112];
    static size_t length;
    static bool discard;
    for (int budget = 256; budget > 0 && Serial.available(); --budget) {
        const char byte = static_cast<char>(Serial.read());
        if (byte == '\n') {
            line[length] = '\0';
            BmsSample next;
            BmsControlState configuration;
            if (!discard && parseBmsSample(line, next)) {
                sample = next;
                lastSample = millis();
                hasSample = true;
                ++validFrames;
            } else if (!discard && bms_parse_state(line, &configuration)) {
                receiveProtection(configuration);
            } else if (discard || strncmp(line, "BMS,TEMP,", 9) != 0) {
                ++rejectedFrames;
            }
            length = 0;
            discard = false;
        } else if (byte != '\r' && !discard) {
            if (byte >= ' ' && byte <= '~' && length < sizeof(line) - 1)
                line[length++] = byte;
            else
                discard = true;
        }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(false);
    thing.add_wifi(BMS_WIFI_SSID, BMS_WIFI_PASSWORD);
    configureProtectionResource();
    ota.on_start([] { otaRunning = true; });
    ota.on_end([] { otaRunning = false; });
    thing["status"] >> [](pson& out) {
        out["firmware"] = "bms-control-v1.1-ota";
        out["ota_enabled"] = ota.is_enabled();
        out["ota_free_bytes"] = ESP.getFreeSketchSpace();
        out["heap_free_bytes"] = ESP.getFreeHeap();
        out["uptime_ms"] = millis();
        out["wifi_rssi_dbm"] = WiFi.RSSI();
        out["valid_frames"] = validFrames;
        out["rejected_frames"] = rejectedFrames;
    };
    thing["telemetry"] >> [](pson& out) {
        const uint32_t age = millis() - lastSample;
        const bool fresh = hasSample && age < 5000u;
        out["data_valid"] = fresh;
        out["temperature_valid"] = fresh;
        out["current_valid"] = fresh && sample.currentMa >= 0;
        if (hasSample) out["sample_age_ms"] = age;
        if (!fresh) return;
        out["sequence"] = sample.sequence;
        out["voltage_v"] = sample.voltageMv / 1000.0;
        out["temperature_c"] = sample.temperatureC;
        out["capacity_percent"] = sample.capacityPercent;
        out["warning_flags"] = sample.warnings;
        out["load_enabled"] = sample.loadEnabled;
        if (sample.currentMa >= 0) {
            out["current_ma"] = sample.currentMa;
            out["power_w"] = sample.voltageMv * sample.currentMa / 1000000.0;
        }
    };
}

void loop() {
    readTelemetry();
    pollProtectionRequest();
    ota.set_enabled(!requestPending);
    thing.handle();
}
