#ifndef BMS_CONTROL_H
#define BMS_CONTROL_H
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
#define BMS_INLINE constexpr
#else
#define BMS_INLINE static inline
#endif

enum {
    BMS_WARNING_LOW_VOLTAGE = 1u << 0,
    BMS_WARNING_HIGH_CURRENT = 1u << 1,
    BMS_WARNING_HIGH_TEMP = 1u << 2,
    BMS_WARNING_CURRENT_SENSOR = 1u << 3,
    BMS_WARNING_TEMPERATURE_SENSOR = 1u << 4,
    BMS_WARNING_MASK = (1u << 5) - 1u
};

BMS_INLINE uint8_t bms_relay_blocking_warnings(uint8_t warnings, uint8_t protection_enabled)
{
    return protection_enabled ? warnings : (uint8_t)(warnings & ~BMS_WARNING_TEMPERATURE_SENSOR);
}

enum {
    BMS_ACK_NONE = 0,
    BMS_ACK_APPLIED = 1,
    BMS_ACK_RANGE = 2,
    BMS_ACK_EDITING = 3,
    BMS_ACK_ID_CONFLICT = 4,
    BMS_ACK_SAVE_ERROR = 5,
    BMS_ACK_REVISION = 6
};

typedef struct {
    uint32_t id, revision, low_mv, high_ma, high_c;
} BmsControlCommand;

typedef struct {
    uint32_t low_mv, high_ma, high_c, revision, ack_id, ack_code;
} BmsControlState;

BMS_INLINE int bms_limits_valid(uint32_t low_mv, uint32_t high_ma, uint32_t high_c)
{
    return low_mv >= 500 && low_mv <= 16500 && low_mv % 100 == 0 &&
        high_ma >= 10 && high_ma <= 3200 && high_c <= 99;
}

BMS_INLINE uint16_t bms_crc16(const char *data, size_t length)
{
    uint16_t crc = 0xffff;
    for (size_t i = 0; i < length; ++i) {
        crc ^= (uint16_t)(uint8_t)data[i] << 8;
        for (unsigned bit = 0; bit < 8; ++bit)
            crc = (uint16_t)((crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1);
    }
    return crc;
}

BMS_INLINE int bms_hex_digit(char byte)
{
    return byte >= '0' && byte <= '9' ? byte - '0' :
        byte >= 'A' && byte <= 'F' ? byte - 'A' + 10 : -1;
}

// Strict decimal fields and CRC; no partial command is ever accepted.
BMS_INLINE int bms_decode_control(const char *line, const char *prefix,
                                  uint32_t *fields, size_t count)
{
    size_t length = 0;
    while (length < 96 && line[length]) ++length;
    if (length >= 96 || length < 6) return 0;
    size_t body = length - 5;
    if (line[body] != '*') return 0;
    uint16_t expected = 0;
    for (size_t i = body + 1; i < length; ++i) {
        int digit = bms_hex_digit(line[i]);
        if (digit < 0) return 0;
        expected = (uint16_t)((expected << 4) | digit);
    }
    if (bms_crc16(line, body) != expected) return 0;
    size_t cursor = 0;
    while (prefix[cursor]) {
        if (cursor >= body || line[cursor] != prefix[cursor]) return 0;
        ++cursor;
    }
    for (size_t field = 0; field < count; ++field) {
        if (cursor >= body || line[cursor] < '0' || line[cursor] > '9') return 0;
        uint32_t value = 0;
        while (cursor < body && line[cursor] >= '0' && line[cursor] <= '9') {
            uint32_t digit = (uint32_t)(line[cursor++] - '0');
            if (value > (UINT32_MAX - digit) / 10u) return 0;
            value = value * 10u + digit;
        }
        fields[field] = value;
        if (field + 1 < count) {
            if (cursor >= body || line[cursor++] != ',') return 0;
        } else if (cursor != body) return 0;
    }
    return 1;
}

BMS_INLINE int bms_parse_command(const char *line, BmsControlCommand *command)
{
    uint32_t f[5] = {0};
    if (!bms_decode_control(line, "BMS,SET,", f, 5) || f[0] == 0) return 0;
    *command = (BmsControlCommand){f[0], f[1], f[2], f[3], f[4]};
    return 1;
}

BMS_INLINE int bms_parse_state(const char *line, BmsControlState *state)
{
    uint32_t f[6] = {0};
    if (!bms_decode_control(line, "BMS,CFG,", f, 6) ||
        !bms_limits_valid(f[0], f[1], f[2]) || f[5] > BMS_ACK_REVISION ||
        ((f[4] == 0) != (f[5] == BMS_ACK_NONE))) return 0;
    *state = (BmsControlState){f[0], f[1], f[2], f[3], f[4], f[5]};
    return 1;
}

static inline int bms_finish_line(char *line, size_t capacity)
{
    size_t length = 0;
    while (length < capacity && line[length]) ++length;
    if (length >= capacity || capacity - length < 7) return 0;
    int appended = snprintf(line + length, capacity - length, "*%04X\n",
                            (unsigned)bms_crc16(line, length));
    return appended == 6 ? (int)length + appended : 0;
}
#undef BMS_INLINE
#endif
