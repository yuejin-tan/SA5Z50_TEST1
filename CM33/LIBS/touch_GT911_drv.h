#pragma once

#include "stdint.h"

typedef struct
{
    struct {
        uint8_t dummy;
        uint8_t id;
        uint16_t x;
        uint16_t y;
        uint16_t size;
    }touchPointInfos[5];

    uint8_t touchPointSta;
    uint8_t nums;
}
GT911info_struct;

int16_t GT9XX_WriteData(uint16_t addr, uint8_t value);
int16_t GT9XX_ReadData(uint16_t addr, uint16_t cnt, uint8_t* value);
void GT911_init(void);
int16_t GT911_Scan(GT911info_struct* hInfo);
