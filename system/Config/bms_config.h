#ifndef BMS_CONFIG_H
#define BMS_CONFIG_H
#include <stdint.h>

extern uint32_t bmsConfigRevision;
void BmsConfig_Init(void);
uint8_t BmsConfig_Apply(uint32_t low_mv, uint32_t high_ma, uint32_t high_c);
#endif
