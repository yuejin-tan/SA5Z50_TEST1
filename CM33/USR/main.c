#include <stdio.h>
#include <stdint.h>
#include "STAR.h"
#include "STAR_gpio.h"
#include "STAR_uart.h"
#include "STAR_rcc.h"

#include "main.h"

#include "ahb_ledseg.h"
#include "ahb_uart.h"

#include "lcd_9488_drv.h"
#include "touch_GT911_drv.h"

#include "delay.h"
#include "scd_inc.h"

// NVIC cfg
static void nvicInit(void)
{
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);


    // NVIC_SetPriority(EXT13_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_2, 1, 0));
    // NVIC_EnableIRQ(EXT13_IRQn);
    // NVIC_ClearPendingIRQ(EXT13_IRQn);


    // NVIC_SetPriority(UART0_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_2, 2, 0));
    // NVIC_EnableIRQ(UART0_IRQn);
    // NVIC_ClearPendingIRQ(UART0_IRQn);
}

//uart初始化配置
static void uartx_init(UART_TypeDef* UARTx, uint32_t BundRate)
{
    UART_InitTypeDef  UART_InitStructure;

    UART_DeInit(UARTx);

    //将对应的GPIO管脚复用为对应UART的收发管脚
    if (UARTx == STAR_UART0)
    {
        GPIO_PinRemapConfig(STAR_GPIO0, GPIO_Remap_USART0_RXD, ENABLE); 					//GPIO[2]
        GPIO_PinRemapConfig(STAR_GPIO0, GPIO_Remap_USART0_TXD, ENABLE); 					//GPIO[3]
    }
    else if (UARTx == STAR_UART1)
    {
        GPIO_PinRemapConfig(STAR_GPIO0, GPIO_Remap_USART1_RXD, ENABLE); 					//GPIO[4]
        GPIO_PinRemapConfig(STAR_GPIO0, GPIO_Remap_USART1_TXD, ENABLE); 					//GPIO[5]
    }
    else
    {
        GPIO_PinRemapConfig(STAR_GPIO0, GPIO_Remap_USART2_RXD, ENABLE); 					//GPIO[22]
        GPIO_PinRemapConfig(STAR_GPIO0, GPIO_Remap_USART2_TXD, ENABLE); 					//GPIO[23]
    }

    //波特率
    UART_InitStructure.UART_BundRate = BundRate;

    //收发使能
    UART_InitStructure.UART_CTRL = UART_CTRL_TxEnable | UART_CTRL_RxEnable;
    UART_Init(UARTx, &UART_InitStructure);

    //接收中断配置
    // UART_ITConfig(UARTx, UART_RxInterrupt, ENABLE);
}

static void uart_ahb_init(void)
{
    AHB_UART->cmd.all = 0;
    AHB_UART->buad_div_2 = (SystemCoreClock / (460800ul * 2ul)) - 1;
    // AHB_UART->buad_div_2 = (SystemCoreClock / (115200UL * 2ul)) - 1;
    AHB_UART->cmd.all = 1;
}

volatile uint32_t millisCnt;
uint16_t test1 = 0;
uint16_t test2 = 0;
uint16_t test3 = 0;
uint16_t test4 = 0;

uint16_t test1r = 0;
uint16_t test2r = 0;
uint16_t test3r = 0;
uint16_t test4r = 0;

static GT911info_struct touchInfo1;

int main(void)
{
    GPIO_DeInit(STAR_GPIO0);
    SysTick_Config(SystemCoreClock / 1000ul - 1ul);

    uartx_init(STAR_UART1, 115200);

    uart_ahb_init();

    nvicInit();

    scd_init_1();

    LCD_Init();

    GT911_init();

    LCD_Draw_Circle(100, 100, 20);
    LCD_Draw_Circle(200, 100, 20);
    LCD_Draw_Circle(100, 200, 20);


    STAR_TIMER1->CTRL = 0;
    STAR_TIMER1->VALUE = 0;
    STAR_TIMER1->RELOAD = SystemCoreClock / 100;
    STAR_TIMER1->INTCLEAR = 1;
    STAR_TIMER1->CTRL = 9;

    while (1)
    {
        while (STAR_TIMER1->INTSTATUS == 0)
        {
            // idle task area
            AHB_LED->seg0 = (millisCnt >> 0) & 0xful;
            AHB_LED->seg1 = (millisCnt >> 4) & 0xful;
            AHB_LED->seg2 = (millisCnt >> 8) & 0xful;
            AHB_LED->seg3 = (millisCnt >> 12) & 0xful;
            AHB_LED->seg4 = (millisCnt >> 16) & 0xful;
            AHB_LED->seg5 = (millisCnt >> 20) & 0xful;
            AHB_LED->seg6 = (millisCnt >> 24) & 0xful;
            AHB_LED->seg7 = (millisCnt >> 28) & 0xful;


            while (AHB_UART->cmd.bit.TX_FIFO_FULL == 0)
            {
                AHB_UART->data = (uint8_t)scd_send1Byte(&scd_1);
            }

            while (AHB_UART->cmd.bit.RX_FIFO_EMPTY == 0)
            {
                SCD_Rev1Byte(&scd_1, AHB_UART->data);
            }
        }
        STAR_TIMER1->INTCLEAR = 1;

        // 10ms tick area
        if (GT911_Scan(&touchInfo1) == 0)
        {
            if (touchInfo1.nums == 2)
            {
                LCD_DrawLine(touchInfo1.touchPointInfos[0].x, touchInfo1.touchPointInfos[0].y,
                    touchInfo1.touchPointInfos[1].x, touchInfo1.touchPointInfos[1].y);
            }
            else
            {
                LCD_DrawPoint_color(touchInfo1.touchPointInfos[0].x, touchInfo1.touchPointInfos[0].y, RED);
            }
        }
    }
}
