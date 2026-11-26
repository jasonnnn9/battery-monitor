#pragma once
#include <stdint.h>
#include <stddef.h>
#include <string.h>

struct BmsSample {
    uint32_t sequence;
    int voltageMv;
    int currentMa;
    int temperatureC;
    int capacityPercent;
    int warnings;
    bool loadEnabled;
};

// Parse into a temporary object so a damaged frame cannot partially update data.
constexpr bool parseBmsSample(const char* line, BmsSample& sample) {
    const char prefix[] = "BMS,V1,";
    for (size_t i = 0; i < sizeof(prefix) - 1; ++i)
        if (line[i] != prefix[i]) return false;
    const char* cursor = line + sizeof(prefix) - 1;
    uint32_t fields[7] = {};
    bool negative[7] = {};
    for (size_t field = 0; field < 7; ++field) {
        if (*cursor == '-') {
            negative[field] = true;
            ++cursor;
        }
        if (*cursor < '0' || *cursor > '9') return false;
        uint32_t value = 0;
        while (*cursor >= '0' && *cursor <= '9') {
            const uint32_t digit = static_cast<uint32_t>(*cursor++ - '0');
            if (value > (UINT32_MAX - digit) / 10u) return false;
            value = value * 10u + digit;
        }
        fields[field] = value;
        if (field < 6) {
            if (*cursor++ != ',') return false;
        } else if (*cursor != '\0') return false;
    }
    if (negative[0] || negative[1] || negative[4] || negative[5] || negative[6]) return false;
    if (fields[1] > 25000 || fields[2] > 65534 || fields[3] > 125 ||
        fields[4] > 100 || fields[5] > 15 || fields[6] > 1) return false;
    if (negative[2] && fields[2] != 1) return false;
    if (negative[3] && fields[3] > 55) return false;
    if (negative[2] != ((fields[5] & 8u) != 0)) return false;
    sample = {fields[0], static_cast<int>(fields[1]),
        negative[2] ? -1 : static_cast<int>(fields[2]),
        negative[3] ? -static_cast<int>(fields[3]) : static_cast<int>(fields[3]),
        static_cast<int>(fields[4]), static_cast<int>(fields[5]), fields[6] != 0};
    return true;
}
