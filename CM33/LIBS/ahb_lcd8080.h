#pragma once

#include "STAR.h"
#include "stdint.h"

typedef struct
{
    __IO uint32_t cmd_reg;
    __IO uint32_t data_reg;

    // bit0:en
    __IO uint8_t ctrl;
    __IO uint8_t tick_cmd_data_chg;
    __IO uint8_t tick_write_l;
    __IO uint8_t tick_write_h;
    __IO uint8_t tick_read_l;
    __IO uint8_t tick_read_h;
	
    __IO uint16_t DUMMY;
}AHB_LCD_TypeDef;

//base address
#define AHB_LCD_BASE   (STAR_TARGEXP1_BASE + 0x00000UL)

//mapping
#define AHB_LCD        ((AHB_LCD_TypeDef   *) AHB_LCD_BASE)

