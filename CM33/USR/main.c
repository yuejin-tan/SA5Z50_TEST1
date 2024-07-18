#include <stdio.h>
#include <stdint.h>
#include "STAR_gpio.h"
#include "STAR_uart.h"
#include "STAR_rcc.h"

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
}

static void delay_ms(uint16_t dly)
{
    volatile uint32_t i, j;
    for (i = 0;i < dly;i++)
    {
        for (j = 0;j < 1000;j++)
        {
            continue;
        }
    }
}


int main(void)
{

    uartx_init(STAR_UART0, 115200);

    while (1)
    {
        printf("STAR uart0 test...\r\n");
        delay_ms(100);
    }
}
