#include "STAR_timer.h"
#include "STAR_rcc.h"

/**
  *
  * @brief: 将定时器寄存器值初始化为复位默认值。
  *
  * @param: TIMERx,使用哪个定时器;由于STAR有2个定时器，因此该参数取值范围为STAR_TIMER0或STAR_TIMER1。
  * 
  * @retVal: void
  */
void TIM_DeInit(TIMER_TypeDef* TIMERx)
{
    assert_param(IS_TIM_ALL_PERIPH(TIMERx));

    if (TIMERx == STAR_TIMER0)
    {
		RCC_APB1PeriphResetCmd( RCC_APB1Periph_TIM0, ENABLE);
		RCC_APB1PeriphResetCmd( RCC_APB1Periph_TIM0, DISABLE);    
    } 
	else if (TIMERx == STAR_TIMER1) 
	{
		RCC_APB1PeriphResetCmd( RCC_APB1Periph_TIM1, ENABLE);
		RCC_APB1PeriphResetCmd( RCC_APB1Periph_TIM1, DISABLE);
    }
}

/**
  *
  * @brief: 将定时器设置参数结构体设置为默认初始值。
  *
  * @param: TIM_TimeBaseInitStruct,需要初始化的定时器设置参数结构体。
  * 
  * @retVal: void
  */
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
    TIM_TimeBaseInitStruct->TIM_Value  = 0x00000000;
	TIM_TimeBaseInitStruct->TIM_Reload = 0x00000000;
}

/**
  *
  * @brief: 定时器初始化设置。
  *
  * @param: TIMERx,使用哪个定时器;由于STAR有2个定时器，因此该参数取值范围为STAR_TIMER0或STAR_TIMER1。
  * @param: TIM_TimeBaseInitStruct,定时器设置参数结构体，该结构体成员变量包括初始加载值和重新加载值。
  * @retVal: void
  */
void TIM_TimeBaseInit(TIMER_TypeDef* TIMERx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
    assert_param(IS_TIM_ALL_PERIPH(TIMERx));

    TIMERx->VALUE  = TIM_TimeBaseInitStruct->TIM_Value;
	TIMERx->RELOAD = TIM_TimeBaseInitStruct->TIM_Reload;
}

/**
  *
  * @brief: 定时器控制寄存器设置。
  *
  * @param: TIMERx,使用哪个定时器;由于STAR有2个定时器，因此该参数取值范围为STAR_TIMER0或STAR_TIMER1。
  * @param: mode,定时器控制寄存器设置值，可设置的值包括：
  *					TIM_ENABLE(0x01) - 使能定时器；
  *					TIM_EXTERNAL_INPUT_AS_ENABLE(0x02) - 选择外部输入作为定时器工作使能；
  *					TIM_EXTERNAL_INPUT_AS_CLOCK(0x04)	 - 选择外部输入作为时钟；
  *					TIM_INTERRUPT_ENABLE(0x08) - 	定时器中断使能。
  * @param: NewState,是否使能设置；ENABLE - 使能；DISABLE - 不使能。
  * @retVal: void
  */
void TIM_CtrlCmd(TIMER_TypeDef* TIMERx, uint8_t mode, FunctionalState NewState) 
{
    
    assert_param(IS_TIM_ALL_PERIPH(TIMERx));
    assert_param(IS_TIME_CTRL_SET(mode));
	
    if (NewState != DISABLE)
    {
       TIMERx->CTRL |= mode;
    }
    else
    {
	   TIMERx->CTRL &= ~mode;
    }
}

/**
  *
  * @brief: 获取定时器中断状态。
  *
  * @param: STAR_TIMERx,使用哪个定时器;由于STAR有2个定时器，因此该参数取值范围为STAR_TIMER0或STAR_TIMER1。
  * 
  * @retVal: bitstatus,定时器中断状态，SET(0x01) -定时器产生了中断；RESET(0x0) - 定时器未产生中断。
  */
ITStatus TIM_GetITStatus(TIMER_TypeDef* TIMERx)
{
    ITStatus bitstatus = RESET;
    uint16_t itstatus = 0x0;
    
    assert_param(IS_TIM_ALL_PERIPH(TIMERx));
	
    itstatus =((uint16_t)TIMERx->INTSTATUS&0x01);
    if (itstatus)
    { 
		    bitstatus = SET; 
	  }
		
    return bitstatus;
}

/**
  *
  * @brief: 清除定时器中断。
  *
  * @param: TIMERx,使用哪个定时器;由于STAR有2个定时器，因此该参数取值范围为STAR_TIMER0或STAR_TIMER1。
  * 
  * @retVal: void
  */
void TIM_ClearIT(TIMER_TypeDef* TIMERx)
{
	
    assert_param(IS_TIM_ALL_PERIPH(TIMERx));

    TIMERx->INTCLEAR |= 0x1;
}

