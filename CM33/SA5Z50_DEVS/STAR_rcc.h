#ifndef __STAR_RCC_H
#define __STAR_RCC_H

#ifdef __cplusplus
 extern "C" {
#endif

/*---------Include---------*/
#include "STAR.h"

/** @defgroup APB1_peripheral 
  * @{
  */
#define RCC_APB1Periph_TIM0     ((uint16_t)0x0001)
#define RCC_APB1Periph_TIM1     ((uint16_t)0x0002)
#define RCC_APB1Periph_DTIMER0  ((uint16_t)0x0004)
#define RCC_APB1Periph_UART0    ((uint16_t)0x0008)
#define RCC_APB1Periph_UART1    ((uint16_t)0x0010)
#define RCC_APB1Periph_I2C      ((uint16_t)0x0020)
#define RCC_APB1Periph_ADC      ((uint16_t)0x0040)
#define RCC_APB1Periph_Wdog     ((uint16_t)0x0080)
#define RCC_APB1Periph_SPI0     ((uint16_t)0x0100)
#define RCC_APB1Periph_SPI1     ((uint16_t)0x0200)
#define RCC_APB1Periph_DTIMER1  ((uint16_t)0x0400)
#define RCC_APB1Periph_UART2    ((uint16_t)0x0800)
#define RCC_APB1Periph_ADC1     ((uint16_t)0x1000)
 
#define IS_RCC_APB1_PERIPH(PERIPH) ((((PERIPH) & 0xE000) == 0x00) && ((PERIPH) != 0x00)) 

/** @defgroup RCC_Exported_Functions
  * @{
  */

void RCC_APB1PeriphResetCmd(uint16_t RCC_APB1Periph, FunctionalState NewState);

/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* __STAR_RCC_H */
