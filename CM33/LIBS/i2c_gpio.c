/**
 * @file i2c.c
 * @brief eeprom存储变量实现，基于GPIO模拟iic
 * @author Tangent (498339337@qq.com)
 * @date 2022-04-08
 *
 * @copyright Copyright (c) 2022 @Tangent
 */

#include "i2c_gpio.h"

#include "STAR.h"
#include "STAR_gpio.h"
#include "STAR_timer.h"

static inline void tyj_i2c_sda_1()
{
    STAR_GPIO0->OUTENCLR = (1ul << GPIO_Pin_16);
}

static inline void tyj_i2c_sda_0()
{
    STAR_GPIO0->OUTENSET = (1ul << GPIO_Pin_16);
}

static inline void tyj_i2c_scl_1()
{
    STAR_GPIO0->OUTENCLR = (1ul << GPIO_Pin_15);
}

static inline void tyj_i2c_scl_0()
{
    STAR_GPIO0->OUTENSET = (1ul << GPIO_Pin_15);
}

static inline uint16_t tyj_i2c_scl_read()
{
    if (STAR_GPIO0->DATA & (1ul << GPIO_Pin_15))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static inline uint16_t tyj_i2c_sda_read()
{
    if (STAR_GPIO0->DATA & (1ul << GPIO_Pin_16))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void hfUnitDelay()
{
    STAR_TIMER0->CTRL = 0;
    STAR_TIMER0->VALUE = STAR_TIMER0->RELOAD;
    STAR_TIMER0->INTCLEAR = 1;
    STAR_TIMER0->CTRL = 9;
    while (STAR_TIMER0->INTSTATUS == 0)
    {
        continue;
    }
    STAR_TIMER0->CTRL = 0;
    STAR_TIMER0->INTCLEAR = 1;
}

void swi2c_init()
{

    STAR_GPIO0->OUTENCLR = (1ul << GPIO_Pin_15);
    STAR_GPIO0->OUTENCLR = (1ul << GPIO_Pin_16);

    STAR_GPIO0->DATAOUT &= ~(1ul << GPIO_Pin_15);
    STAR_GPIO0->DATAOUT &= ~(1ul << GPIO_Pin_16);

    STAR_TIMER0->CTRL = 0;
    STAR_TIMER0->RELOAD = SystemCoreClock / 100000 / 2;
    STAR_TIMER0->VALUE = STAR_TIMER0->RELOAD;
    STAR_TIMER0->INTCLEAR = 1;
}

int16_t start_i2c()
{
    tyj_i2c_scl_0();
    hfUnitDelay();
    tyj_i2c_sda_1();
    hfUnitDelay();
    tyj_i2c_scl_1();
    hfUnitDelay();
    if (tyj_i2c_scl_read() == 0)
    {
        return -__LINE__;
    }
    if (tyj_i2c_sda_read() == 0)
    {
        return -__LINE__;
    }
    tyj_i2c_sda_0();
    hfUnitDelay();
    tyj_i2c_scl_0();
    hfUnitDelay();
    return 0;
}

int16_t write_i2c(uint16_t data)
{
    for (int ii = 7; ii >= 0; ii--)
    {
        int bitTmp = (data & (1ul << ii)) ? 1 : 0;
        if (bitTmp)
        {
            tyj_i2c_sda_1();
        }
        else
        {
            tyj_i2c_sda_0();
        }
        hfUnitDelay();
        tyj_i2c_scl_1();
        hfUnitDelay();
        if (tyj_i2c_scl_read() == 0)
        {
            return -__LINE__;
        }
        int bitCheckRead = tyj_i2c_sda_read();
        if (bitTmp != bitCheckRead)
        {
            return -__LINE__;
        }
        tyj_i2c_scl_0();
        hfUnitDelay();
    }
    tyj_i2c_sda_1();
    hfUnitDelay();
    tyj_i2c_scl_1();
    hfUnitDelay();
    uint16_t ret = tyj_i2c_sda_read();
    tyj_i2c_scl_0();
    hfUnitDelay();
    return (int16_t)ret;
}

int16_t read_i2c(uint16_t ack)
{
    uint16_t ret = 0;
    tyj_i2c_sda_1();
    for (int ii = 7; ii >= 0; ii--)
    {
        hfUnitDelay();
        tyj_i2c_scl_1();
        hfUnitDelay();
        if (tyj_i2c_scl_read() == 0)
        {
            return -__LINE__;
        }
        ret = (uint16_t)(ret << 1) | tyj_i2c_sda_read();
        tyj_i2c_scl_0();
        hfUnitDelay();
    }
    if (ack)
    {
        tyj_i2c_sda_1();
    }
    else
    {
        tyj_i2c_sda_0();
    }
    hfUnitDelay();
    tyj_i2c_scl_1();
    hfUnitDelay();
    if (tyj_i2c_sda_read() != ack)
    {
        return -__LINE__;
    }
    tyj_i2c_scl_0();
    hfUnitDelay();
    return (int16_t)ret;
}

int16_t stop_i2c()
{
    tyj_i2c_sda_0();
    hfUnitDelay();
    tyj_i2c_scl_1();
    hfUnitDelay();
    tyj_i2c_sda_1();
    hfUnitDelay();
    if (tyj_i2c_scl_read() == 0)
    {
        return -__LINE__;
    }
    if (tyj_i2c_sda_read() == 0)
    {
        return -__LINE__;
    }
    return 0;
}
