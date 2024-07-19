#pragma once

#include "STAR.h"
#include "stdint.h"

typedef struct
{
    __IO   uint8_t  seg0;
    __IO   uint8_t  seg1;
    __IO   uint8_t  seg2;
    __IO   uint8_t  seg3;
    __IO   uint8_t  seg4;
    __IO   uint8_t  seg5;
    __IO   uint8_t  seg6;
    __IO   uint8_t  seg7;
}AHB_LED_TypeDef;

//base address
#define AHB_LED_BASE   (STAR_TARGEXP1_BASE + 0x10000UL)

//mapping
#define AHB_LED        ((AHB_LED_TypeDef   *) AHB_LED_BASE)

