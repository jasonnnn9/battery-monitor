#pragma once
#include <math.h>
#include "bms_control.h"

static BmsControlState protectionState;
static bool hasProtection;
static uint32_t protectionReceived;
static bool requestPending;
static uint32_t requestId;
static uint32_t nextRequestId;
static uint32_t requestStarted, requestLastSent;
static char requestLine[80];
static int requestLength;
static bool otaRunning;
static const char* requestStatus = "idle";

static bool protectionFresh() {
    return hasSample && hasProtection && millis() - lastSample < 5000u &&
        millis() - protectionReceived < 5000u;
}

static const char* acknowledgementName(uint32_t code) {
    switch (code) {
    case BMS_ACK_APPLIED: return "applied";
    case BMS_ACK_RANGE: return "invalid_limits";
    case BMS_ACK_EDITING: return "local_editing";
    case BMS_ACK_ID_CONFLICT: return "request_id_conflict";
    case BMS_ACK_SAVE_ERROR: return "save_failed";
    case BMS_ACK_REVISION: return "configuration_changed";
    default: return "idle";
    }
}

static void receiveProtection(const BmsControlState& state) {
    protectionState = state;
    protectionReceived = millis();
    hasProtection = true;
    if (requestId && state.ack_id == requestId && state.ack_code != BMS_ACK_NONE) {
        requestPending = false;
        requestStatus = acknowledgementName(state.ack_code);
    }
}

static void pollProtectionRequest() {
    if (!requestPending) return;
    const uint32_t now = millis();
    if (now - requestStarted >= 8000u) {
        requestPending = false;
        requestStatus = "timeout";
    } else if (now - requestLastSent >= 2500u) {
        Serial.write(reinterpret_cast<const uint8_t*>(requestLine), requestLength);
        requestLastSent = now;
    }
}

static void outputProtection(pson& out) {
    const bool online = protectionFresh();
    out["board_online"] = online;
    out["persistent"] = true;
    out["request_pending"] = requestPending;
    out["request_id"] = requestId;
    out["request_status"] = requestStatus;
    if (online) {
        out["low_voltage_v"] = protectionState.low_mv / 1000.0;
        out["high_current_ma"] = protectionState.high_ma;
        out["high_temperature_c"] = protectionState.high_c;
        out["revision"] = protectionState.revision;
    }
}

static void configureProtectionResource() {
    nextRequestId = ESP.getChipId() ^ micros();
    thing["protection"] = [](pson& in, pson& out) {
        if (in.is_empty()) { outputProtection(out); return; }
        const char* error = nullptr;
        if (otaRunning) error = "ota_in_progress";
        else if (requestPending) error = "request_pending";
        else if (!protectionFresh()) error = "board_offline";
        else if (!in.is_object() || !in["low_voltage_v"].is_number() ||
                 !in["high_current_ma"].is_number() || !in["high_temperature_c"].is_number())
            error = "three_numeric_limits_required";
        if (error) {
            outputProtection(out);
            out["error"] = error;
            return;
        }
        const double volts = in["low_voltage_v"];
        const double ma = in["high_current_ma"];
        const double degrees = in["high_temperature_c"];
        if (!isfinite(volts) || !isfinite(ma) || !isfinite(degrees) ||
            volts < 0.5 || volts > 16.5 || ma < 10 || ma > 3200 || degrees < 0 || degrees > 99 ||
            floor(ma) != ma || floor(degrees) != degrees) {
            outputProtection(out); out["error"] = "invalid_limits"; return;
        }
        const uint32_t mv = static_cast<uint32_t>(volts * 1000.0 + 0.5);
        if (fabs(volts - mv / 1000.0) > 0.00001 || !bms_limits_valid(mv, ma, degrees)) {
            outputProtection(out); out["error"] = "voltage_requires_0_1_v_steps"; return;
        }
        requestId = ++nextRequestId;
        if (!requestId) requestId = ++nextRequestId;
        requestLength = snprintf(requestLine, sizeof(requestLine),
            "BMS,SET,%lu,%lu,%lu,%lu,%lu", static_cast<unsigned long>(requestId),
            static_cast<unsigned long>(protectionState.revision), static_cast<unsigned long>(mv),
            static_cast<unsigned long>(ma), static_cast<unsigned long>(degrees));
        requestLength = requestLength > 0 && static_cast<size_t>(requestLength) < sizeof(requestLine) ?
            bms_finish_line(requestLine, sizeof(requestLine)) : 0;
        if (!requestLength) {
            outputProtection(out); out["error"] = "command_encoding_failed"; return;
        }
        requestPending = true;
        requestStatus = "pending";
        requestStarted = requestLastSent = millis();
        ota.set_enabled(false);
        if (Serial.write(reinterpret_cast<const uint8_t*>(requestLine), requestLength) !=
            static_cast<size_t>(requestLength)) {
            requestPending = false;
            requestStatus = "uart_write_failed";
        }
        outputProtection(out);
    };
}
