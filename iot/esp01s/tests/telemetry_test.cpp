#include "../BatteryMonitorThinger/telemetry.h"

constexpr bool parses(const char* line) {
    BmsSample sample = {};
    return parseBmsSample(line, sample);
}
constexpr bool valuesMatch() {
    BmsSample sample = {};
    return parseBmsSample("BMS,V1,42,4240,1,27,100,0,1", sample) &&
        sample.sequence == 42 && sample.voltageMv == 4240 &&
        sample.currentMa == 1 && sample.temperatureC == 27 &&
        sample.capacityPercent == 100 && sample.warnings == 0 && sample.loadEnabled;
}
constexpr bool preservesOnFailure() {
    BmsSample sample = {42, 4240, 1, 27, 100, 0, true};
    return !parseBmsSample("BMS,V1,1,4240,0,27,101,0,0", sample) && sample.sequence == 42;
}
static_assert(valuesMatch(), "Measurement units/fields must match the sender");
static_assert(preservesOnFailure(), "Invalid frames must not partially update data");
static_assert(parses("BMS,V1,4294967295,25000,65534,125,100,7,0"), "Upper bounds");
static_assert(parses("BMS,V1,0,0,-1,-55,0,8,0"), "Sensor failure and negative temperature");
static_assert(!parses(""), "Empty input");
static_assert(!parses("BMS,TEMP,27"), "Temperature compatibility line is not a full frame");
static_assert(!parses("BMS,V2,1,4240,0,27,100,0,0"), "Unknown version");
static_assert(!parses("BMS,V1,4294967296,4240,0,27,100,0,0"), "Overflow");
static_assert(!parses("BMS,V1,-1,4240,0,27,100,0,0"), "Negative sequence");
static_assert(!parses("BMS,V1,1,-4240,0,27,100,0,0"), "Negative voltage");
static_assert(!parses("BMS,V1,1,25001,0,27,100,0,0"), "Voltage range");
static_assert(!parses("BMS,V1,1,4240,-2,27,100,8,0"), "Invalid sentinel");
static_assert(!parses("BMS,V1,1,4240,-1,27,100,0,0"), "Missing sensor alarm");
static_assert(!parses("BMS,V1,1,4240,0,27,100,8,0"), "Sensor alarm mismatch");
static_assert(!parses("BMS,V1,1,4240,0,-56,100,0,0"), "Temperature range");
static_assert(!parses("BMS,V1,1,4240,0,27,101,0,0"), "Capacity range");
static_assert(!parses("BMS,V1,1,4240,0,27,100,16,0"), "Unknown alarm bits");
static_assert(!parses("BMS,V1,1,4240,0,27,100,0,2"), "Load range");
static_assert(!parses("BMS,V1,1,4240,0,27,100,0"), "Missing field");
static_assert(!parses("BMS,V1,1,4240,0,27,100,0,0,1"), "Extra field");
static_assert(!parses("BMS,V1,1,4240,0,27,100,0,0junk"), "Trailing junk");
static_assert(!parses("BMS,V1,1,4240,,27,100,0,0"), "Empty field");
