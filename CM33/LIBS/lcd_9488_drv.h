#pragma once

#include "stdlib.h"
#include "stdint.h"

#include "STAR.h"
#include "STAR_gpio.h"
#include "ahb_lcd8080.h"

//////////////////////////////////////////////////////////////////////////////////
// TFT液晶驱动
// 正点原子@ALIENTEK
// modified by tyj
//////////////////////////////////////////////////////////////////////////////////

//LCD的画笔颜色和背景色
extern uint32_t  POINT_COLOR;//默认红色
extern uint32_t  BACK_COLOR; //背景颜色.默认为白色

// 内部命令地址
#define WRITE_RAM_CMD 0X2CUL
#define SETX_CMD 0X2AUL
#define SETY_CMD 0X2BUL

// LCD 参数
#define LCD_H 320
#define LCD_W 480

//写寄存器函数
//regval:寄存器值
static inline void LCD_WR_REG(uint16_t regval)
{
    AHB_LCD->cmd_reg = regval;
}

//写LCD数据
//data:要写入的值
static inline void LCD_WR_DATA(uint16_t data)
{
    AHB_LCD->data_reg = data;
}

//读LCD数据
//返回值:读到的值
static inline uint16_t LCD_RD_DATA(void)
{
    return (uint16_t)AHB_LCD->data_reg;
}

//开始写GRAM
static inline void LCD_WriteRAM_Prepare(void)
{
    AHB_LCD->cmd_reg = WRITE_RAM_CMD;
}

//LCD写GRAM
//RGB_Code:颜色值
static inline void LCD_WriteRAM(uint16_t RGB_Code)
{
    AHB_LCD->data_reg = RGB_Code;//写十六位GRAM
}

//LCD开启显示
static inline void LCD_DisplayOn(void)
{
    LCD_WR_REG(0X29);
}

//LCD关闭显示
static inline void LCD_DisplayOff(void)
{
    LCD_WR_REG(0X28);
}

//扫描方向定义
#define L2R_U2D 0 //从左到右,从上到下
#define L2R_D2U 1 //从左到右,从下到上
#define R2L_U2D 2 //从右到左,从上到下
#define R2L_D2U 3 //从右到左,从下到上

#define U2D_L2R 4 //从上到下,从左到右
#define U2D_R2L 5 //从上到下,从右到左
#define D2U_L2R 6 //从下到上,从左到右
#define D2U_R2L 7 //从下到上,从右到左

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 

#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_Init(void);													        //初始化
void LCD_DisplayOn(void);													    //开显示
void LCD_DisplayOff(void);													    //关显示
void LCD_Clear(uint32_t Color);	 												//清屏
void LCD_Scan_Dir(uint8_t dir);									                //设置屏扫描方向
void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);	//设置窗口
void LCD_DrawPoint_color(uint16_t x, uint16_t y, uint32_t color);				//画点
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);						//画圆
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);			//画线
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		//画矩形
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t mode);			//显示一个字符
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len);  			//显示一个数字
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t mode);			//显示 数字
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* p);	//显示一个字符串,12/16字体
