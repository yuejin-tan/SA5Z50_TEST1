#include "touch_GT911_drv.h"

#include "stdio.h"
#include "STAR_rcc.h"
#include "STAR_gpio.h"
#include "STAR.h"

#include "delay.h"
#include "i2c_gpio.h"

#define GT9XX_IIC_WADDR 0xBAUL
#define GT9XX_IIC_RADDR 0xBBUL

#define GT9XX_ID_ADDR           0x8140U // ID of touch IC
#define GT9XX_X_RES_ADDR        0x8146U // X coordinate resolution
#define GT9XX_Y_RES_ADDR        0x8148U // Y coordinate resolution
#define GT9XX_STATUS_ADDR       0x814EU // touch point STA
#define GT9XX_FIRST_ADDR        0x814FU // touch point 0 ADDR
#define GT9XX_POINT_INFO_OFFSET 0x8U    // touch point 0 ADDR

int16_t GT9XX_WriteData(uint16_t addr, uint8_t value)
{
    int16_t ans;

    ans = start_i2c();
    if (ans < 0)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    ans = write_i2c(GT9XX_IIC_WADDR);
    if (ans < 0 || ans == 1)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    ans = write_i2c((uint8_t)(addr >> 8));
    if (ans < 0 || ans == 1)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    ans = write_i2c((uint8_t)(addr & 0xffu));
    if (ans < 0 || ans == 1)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    ans = write_i2c(value);
    if (ans < 0 || ans == 1)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    stop_i2c();
    return 0;
}

int16_t GT9XX_ReadData(uint16_t addr, uint16_t cnt, uint8_t* value)
{
    int16_t ans;

    ans = start_i2c();
    if (ans < 0)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    ans = write_i2c(GT9XX_IIC_WADDR);
    if (ans < 0 || ans == 1)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    ans = write_i2c((uint8_t)(addr >> 8));
    if (ans < 0 || ans == 1)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    ans = write_i2c((uint8_t)(addr & 0xffu));
    if (ans < 0 || ans == 1)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    ans = stop_i2c();
    if (ans < 0)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    ans = start_i2c();
    if (ans < 0)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    ans = write_i2c(GT9XX_IIC_RADDR);
    if (ans < 0 || ans == 1)
    {
        stop_i2c();
        return -__LINE__ - 1000;
    }

    for (uint16_t i = 0; i < cnt; i++)
    {
        if (i == (cnt - 1))
        {
            ans = read_i2c(1);
            if (ans < 0)
            {
                stop_i2c();
                return -__LINE__ - 1000;
            }
            value[i] = (uint8_t)ans;
        }
        else
        {
            ans = read_i2c(0);
            if (ans < 0)
            {
                stop_i2c();
                return -__LINE__ - 1000;
            }
            value[i] = (uint8_t)ans;
        }
    }

    stop_i2c();
    return 0;
}


void GT911_init(void)
{
    // 复位触摸芯片
    GPIO_ResetBit(STAR_GPIO0, GPIO_Pin_18);
    GPIO_ResetBit(STAR_GPIO0, GPIO_Pin_21);
    delay_ms(10);

    // 初始化iic
    swi2c_init();
    // 设置地址为0xBA
    GPIO_SetBit(STAR_GPIO0, GPIO_Pin_18);
    delay_ms(10);
    GPIO_OutModeDisable(STAR_GPIO0, GPIO_Pin_21);

    uint8_t touchIC_ID[4];
    GT9XX_ReadData(GT9XX_ID_ADDR, 4, touchIC_ID);
    printf("Touch ID: %s\r\n", touchIC_ID);

    uint16_t uint16val;
    GT9XX_ReadData(GT9XX_X_RES_ADDR, 2, (uint8_t*)&uint16val);
    printf("x coordinate resolution: %d\r\n", uint16val);

    GT9XX_ReadData(GT9XX_Y_RES_ADDR, 2, (uint8_t*)&uint16val);
    printf("y coordinate resolution: %d\r\n", uint16val);
}

// #define TOUCH_911_DBG

int16_t GT911_Scan(GT911info_struct* hInfo)
{
    int16_t ans = GT9XX_ReadData(GT9XX_STATUS_ADDR, 1, (uint8_t*)&hInfo->touchPointSta);
    hInfo->nums = 0;

    if (ans < 0)
    {
#ifdef TOUCH_911_DBG
        printf("err@l%d\r\n", __LINE__);
#endif
        return -__LINE__ - 1000;
    }

    if (hInfo->touchPointSta & 0X80)
    {
        hInfo->nums = hInfo->touchPointSta & 0xfU;
#ifdef TOUCH_911_DBG
        printf("ok:%dpts\r\n", hInfo->nums);
#endif
    }
    else
    {
#ifdef TOUCH_911_DBG
        printf("no data\r\n");
#endif
        return -__LINE__ - 1000;
    }

    for (uint16_t i = 0;i < hInfo->nums;i++)
    {
        ans = GT9XX_ReadData((uint16_t)(GT9XX_FIRST_ADDR + i * GT9XX_POINT_INFO_OFFSET), 7, (uint8_t*)&(hInfo->touchPointInfos[i].id));
        if (ans < 0)
        {
#ifdef TOUCH_911_DBG
            printf("err@l%d\r\n", __LINE__);
#endif
            return -__LINE__ - 1000;
        }
#ifdef TOUCH_911_DBG
        printf("[%d]:%d#(%d,%d)@%d\r\n", i, hInfo->touchPointInfos[i].id,
            hInfo->touchPointInfos[i].x, hInfo->touchPointInfos[i].y, hInfo->touchPointInfos[i].size);
#endif
    }

    ans = GT9XX_WriteData(GT9XX_STATUS_ADDR, 0);
    if (ans < 0)
    {
#ifdef TOUCH_911_DBG
        printf("err@l%d\r\n", __LINE__);
        return -__LINE__ - 1000;
#endif
    }

    return 0;
}

