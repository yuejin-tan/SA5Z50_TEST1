#ifndef __STAR_TIMER_H
#define __STAR_TIMER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "STAR.h"


/** 
  * @brief  TIM Time Base Init structure definition
  * @note   This structure is used with all TIMx except for TIM6 and TIM7.    
  */

typedef struct
{
	//uint8_t  TIM_Ctrl;
    uint32_t TIM_Value;
    uint32_t TIM_Reload;   
    
} TIM_TimeBaseInitTypeDef;  

#define TIM_ENABLE                       		   	(0x1)
#define TIM_EXTERNAL_INPUT_AS_ENABLE				(0x2)
#define TIM_EXTERNAL_INPUT_AS_CLOCK                	(0x4)
#define TIM_INTERRUPT_ENABLE                			 (0x8)
#define IS_TIME_CTRL_SET(SET) 		(	((SET) == TIM_ENABLE) || \
																		((SET) == TIM_EXTERNAL_INPUT_AS_ENABLE) || \
																		((SET) == TIM_EXTERNAL_INPUT_AS_CLOCK) || \
																		((SET) == TIM_INTERRUPT_ENABLE))

/** @defgroup TIM_Exported_constants 
  * @{
  */

#define IS_TIM_ALL_PERIPH(PERIPH)  (((PERIPH) == STAR_TIMER0 ) ||((PERIPH) == STAR_TIMER1))

/** @defgroup TIM_Exported_Functions
  * @{
  */
void TIM_DeInit(TIMER_TypeDef* TIMERx);
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
void TIM_TimeBaseInit(TIMER_TypeDef* TIMERx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);

void TIM_CtrlCmd(TIMER_TypeDef* TIMERx,uint8_t mode,FunctionalState NewState);
   
ITStatus TIM_GetITStatus(TIMER_TypeDef*  TIMERx);
void TIM_ClearIT(TIMER_TypeDef*  TIMERx);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STAR_TIMER_H */

