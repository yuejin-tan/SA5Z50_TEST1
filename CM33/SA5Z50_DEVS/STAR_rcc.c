#include "STAR_rcc.h"
#include "STAR.h"

/**
  *
  * @brief: 对指定的APB总线外设的时钟进行复位。
  *
  * @param: RCC_APB1Periph,需要进行时钟复位的APB总线外设；
  * 			可设置的值包括：RCC_APB1Periph_TIM0; RCC_APB1Periph_TIM1;
  *				RCC_APB1Periph_DTIMER0; RCC_APB1Periph_UART0 - UART0; RCC_APB1Periph_UART1;
  *				RCC_APB1Periph_I2C; RCC_APB1Periph_ADC; RCC_APB1Periph_Wdog;
  *				RCC_APB1Periph_SPI0; RCC_APB1Periph_SPI1.
  *					
  * @param: NewState,是否进行复位操作；ENABLE - 复位；DISABLE - 复位释放。
  * 
  * @retVal: void
  */
void RCC_APB1PeriphResetCmd(uint16_t RCC_APB1Periph, FunctionalState NewState)
{
    /*Checking the parameters*/
    assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph) );
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
         APB_PERI_RSTCTL->RESETPERI &= ~RCC_APB1Periph;
    }
    else
    {
         APB_PERI_RSTCTL->RESETPERI |= RCC_APB1Periph;
    }
}
