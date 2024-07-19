#include "touch_GT911_drv.h"

#include "STAR_rcc.h"
#include "STAR_gpio.h"
#include "STAR.h"
#include "STAR_i2c.h"


#define I2C_OWN_ADDR 0x00

#define GT911_W_ADDR 0xBA
#define GT911_R_ADDR 0xBB

void GT911_init(void)
{

    I2C_DeInit(STAR_I2C);
    //将对应的GPIO复用为I2C的管脚
    GPIO_PinRemapConfig(STAR_GPIO0, GPIO_Remap_I2C0SCL, ENABLE); //GPIO[15]
    GPIO_PinRemapConfig(STAR_GPIO0, GPIO_Remap_I2C0SDA, ENABLE); //GPIO[16]
    //I2C配置: I2C主模式，标准I2C速率	
    I2C_Init(STAR_I2C, I2C_Mode_Master, I2C_Speed_Standard, I2C_OWN_ADDR);
    //使能发送
    I2C_TxEnable(STAR_I2C, ENABLE);
    //设置设备地址
    I2C_SlvaddrConfig(STAR_I2C, GT911_W_ADDR);
    //开始发送设备地址和数据
    I2C_GenerateSTART(STAR_I2C, ENABLE);

    I2C_TxRxDataNumConfig(STAR_I2C, 2);

    I2C_SendData(STAR_I2C, 0X11); 	//24C64 wordAddr 是16bit 分两次发送
    while (I2C_CheckACKIsFail(STAR_I2C));	//等待ACK
    

}


