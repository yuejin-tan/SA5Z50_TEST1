#include  "STAR_dualtimer.h"
#include  "STAR_rcc.h"

/**
  *
  * @brief: 将DualTimer寄存器值初始化为复位默认值。
  *
  * @param: DUALTIMERx,设置为STAR_DUALTIMER0或STAR_DUALTIMER1。
  * 
  * @retVal: void
  */
void DTIM_DeInit(DUALTIMER_TypeDef* DUALTIMERx)
{	
	assert_param(IS_DUALTIMER_ALL_PERIPH(DUALTIMERx));
	
	if (DUALTIMERx == STAR_DUALTIMER0)
	{
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_DTIMER0, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_DTIMER0, DISABLE);
	}
	else if (DUALTIMERx == STAR_DUALTIMER1)
	{
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_DTIMER1, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_DTIMER1, DISABLE);
	}
}

/**
  *
  * @brief: DualTimer初始化设置。
  *
  * @param: DUALTIMERx,设置为STAR_DUALTIMER0或STAR_DUALTIMER1。
  * @param: DTIM,双定时器下的哪个定时器,双定时器共有两个定时器可供选择,DTIM_DUALTIMERx_1或DTIM_DUALTIMERx_2。
  * @param: DTIM_SIZE,选择16位或者32位的计数器，取值为DTIM_SIZE_16bit或DTIM_SIZE_32bit。
  * @param: DTIM_TIMERPRE,时钟分频系数,取值为DTIM_TIMERPRE_1或DTIM_TIMERPRE_16或DTIM_TIMERPRE_256。
  * @param: LOAD,定时器初始加载值。
  *
  * @retVal: void
  */
void DTIM_Init(DUALTIMER_TypeDef* DUALTIMERx, uint32_t DTIM, uint32_t DTIM_SIZE, uint32_t DTIM_TIMERPRE, uint32_t LOAD)
{		
	assert_param(IS_DUALTIMER_ALL_PERIPH(DUALTIMERx));
	assert_param(IS_DTIM_CLOCK_SEQUENCE(DTIM));
	assert_param(IS_DTIM_TIMER_SIZE(SIZE));
	assert_param(IS_DTIM_TIMER_PRE(PRE));
		
	
	if(DTIM == DTIM_DUALTIMERx_1)
	{
		DUALTIMERx->Timer1Control  = 0;
		DUALTIMERx->Timer1Control |=  (DTIM_SIZE | DTIM_TIMERPRE);
		DUALTIMERx->Timer1Load 	  = LOAD;
		
	}
	else if(DTIM == DTIM_DUALTIMERx_2)
	{
		DUALTIMERx->Timer2Control  = 0;
		DUALTIMERx->Timer2Control |= (DTIM_SIZE | DTIM_TIMERPRE);
		DUALTIMERx->Timer2Load 	  = LOAD;
		
	}
}

/**
  *
  * @brief: DualTimer工作模式设置。
  *
  * @param: DUALTIMERx,设置为STAR_DUALTIMER0或STAR_DUALTIMER1。
  * @param: DTIM,双定时器下的哪个定时器,双定时器共有两个定时器可供选择,DTIM_DUALTIMERx_1或DTIM_DUALTIMERx_2。
  * @param: DTIM_MODE,工作模式；
  * 					可设置的值包括：
  *							DTIM_MODE_ONE_SHOT_COUNT - 单次计时模式；
  *							DTIM_MODE_FREE_RUNNING - 自由运行模式；
  *							DTIM_MODE_PERIODIC - 周期计时模式。
  *
  * @retVal: void
  */
void DTIM_MODE(DUALTIMER_TypeDef* DUALTIMERx, uint32_t DTIM, uint32_t DTIM_MODE)
{		
	assert_param(IS_DUALTIMER_ALL_PERIPH(DUALTIMERx));
	assert_param(IS_DTIM_CLOCK_SEQUENCE(DTIM));
	assert_param(IS_DTIM_WORK_MODE(DTIM_MODE));
	
	if(DTIM == DTIM_DUALTIMERx_1)
	{
		DUALTIMERx->Timer1Control |=  DTIM_MODE;
	}
	else if(DTIM == DTIM_DUALTIMERx_2)
	{
		DUALTIMERx->Timer2Control |=  DTIM_MODE;
	}
}

/**
  *
  * @brief: DualTimer使能设置。
  *
  * @param: DUALTIMERx,设置为STAR_DUALTIMER0或STAR_DUALTIMER1。
  * @param: DTIM,双定时器下的哪个定时器,双定时器共有两个定时器可供选择,DTIM_DUALTIMERx_1或DTIM_DUALTIMERx_2。
  * @param: NewState,是否使能DualTimer;ENABLE：使能， DISABLE：不使能。
  *
  * @retVal: void
  */
void DTIM_ENABLE(DUALTIMER_TypeDef* DUALTIMERx, uint32_t DTIM, FunctionalState NewState)
{	
	assert_param(IS_DUALTIMER_ALL_PERIPH(DUALTIMERx));
	assert_param(IS_DTIM_CLOCK_SEQUENCE(DTIM));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if((DTIM == DTIM_DUALTIMERx_1)&&(NewState != DISABLE))
	{
		DUALTIMERx->Timer1Control |= STAR_DUALTIMER1_CTRL_EN_Msk; 
	}
	else if((DTIM == DTIM_DUALTIMERx_2)&&(NewState != DISABLE))
	{
		DUALTIMERx->Timer2Control |= STAR_DUALTIMER2_CTRL_EN_Msk;
	}
	else
	{
		if(DTIM == DTIM_DUALTIMERx_1)
		{
			DUALTIMERx->Timer1Control &= ~STAR_DUALTIMER1_CTRL_EN_Msk;
		}
		else
		{
			DUALTIMERx->Timer2Control &= ~STAR_DUALTIMER2_CTRL_EN_Msk;
		}
	}
}

/**
  *
  * @brief: 设置DualTimer的重新加载值。
  *
  * @param: DUALTIMERx,设置为STAR_DUALTIMER0或STAR_DUALTIMER1。
  * @param: DTIM,双定时器下的哪个定时器,双定时器共有两个定时器可供选择,DTIM_DUALTIMERx_1或DTIM_DUALTIMERx_2。
  * @param: BGLOAD,重新加载值。
  *
  * @retVal: void
  */
void DTIM_SetBGLOAD(DUALTIMER_TypeDef* DUALTIMERx, uint32_t DTIM, uint32_t BGLOAD)
{
	assert_param(IS_DUALTIMER_ALL_PERIPH(DUALTIMERx));
	assert_param(IS_DTIM_CLOCK_SEQUENCE(DTIM));
	
	if(DTIM == DTIM_DUALTIMERx_1)
	{
		DUALTIMERx->Timer1BGLoad  = BGLOAD;
	}
	else if(DTIM == DTIM_DUALTIMERx_2)
	{
		DUALTIMERx->Timer2BGLoad  = BGLOAD;
	}
}

/**
  *
  * @brief: DualTimer中断使能设置。
  *
  * @param: DUALTIMERx,设置为STAR_DUALTIMER0或STAR_DUALTIMER1。
  * @param: DTIM,双定时器下的哪个定时器,双定时器共有两个定时器可供选择,DTIM_DUALTIMERx_1或DTIM_DUALTIMERx_2。
  * @param: NewState,是否使能DualTimer中断;ENABLE：使能， DISABLE：不使能。
  *
  * @retVal: void
  */
void DTIM_ITConfig(DUALTIMER_TypeDef* DUALTIMERx, uint32_t DTIM, FunctionalState NewState)
{
    
    assert_param(IS_DUALTIMER_ALL_PERIPH(DUALTIMERx));
	assert_param(IS_DTIM_CLOCK_SEQUENCE(DTIM));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if((DTIM == DTIM_DUALTIMERx_1)&&(NewState != DISABLE))
	{	
		
		DUALTIMERx->Timer1Control |=  STAR_DUALTIMER1_CTRL_INTEN_Msk;
	}
	else if((DTIM == DTIM_DUALTIMERx_2)&&(NewState != DISABLE))
	{
		DUALTIMERx->Timer2Control |= STAR_DUALTIMER2_CTRL_INTEN_Msk ;
	}
	else
	{
		if(DTIM == DTIM_DUALTIMERx_1)
		{
			DUALTIMERx->Timer1Control &= (~STAR_DUALTIMER1_CTRL_INTEN_Msk);
		}
		else
		{
			DUALTIMERx->Timer2Control &= (~STAR_DUALTIMER2_CTRL_INTEN_Msk) ;
		}
	}
}

/**
  *
  * @brief: 清除DualTimer中断。
  *
  * @param: DUALTIMERx,设置为STAR_DUALTIMER0或STAR_DUALTIMER1。
  * @param: DTIM,双定时器下的哪个定时器,双定时器共有两个定时器可供选择,DTIM_DUALTIMERx_1或DTIM_DUALTIMERx_2。
  * @param: NewState,是否清除DualTimer中断;ENABLE：清除， DISABLE：不清除。
  *
  * @retVal: void
  */
void DTIM_ClearIT(DUALTIMER_TypeDef* DUALTIMERx, uint32_t DTIM, FunctionalState NewState)
{			
	
	assert_param(IS_DUALTIMER_ALL_PERIPH(DUALTIMERx));
	assert_param(IS_DTIM_CLOCK_SEQUENCE(DTIM));
	
	
	if((DTIM == DTIM_DUALTIMERx_1)&&(NewState != DISABLE))
	{
		DUALTIMERx->Timer1IntClr |= STAR_DUALTIMER1_INTCLR_Msk;
	}
	else if((DTIM == DTIM_DUALTIMERx_2)&&(NewState != DISABLE))
	{
		DUALTIMERx->Timer2IntClr |= STAR_DUALTIMER2_INTCLR_Msk;
	}
	else
	{
		//do nothing
	}	
}

/**
  *
  * @brief: 查看DualTimer计数器当前值。
  *
  * @param: DUALTIMERx,设置为STAR_DUALTIMER0或STAR_DUALTIMER1。
  * @param: DTIM,双定时器下的哪个定时器,双定时器共有两个定时器可供选择,DTIM_DUALTIMERx_1或DTIM_DUALTIMERx_2。
  *
  * @retVal: value,获取到的计数器当前值。
  */
uint32_t DTIM_Timer0CurrentValue(DUALTIMER_TypeDef* DUALTIMERx, uint32_t DTIM)
{	
	uint32_t value = 0;
	assert_param(IS_DUALTIMER_ALL_PERIPH(DUALTIMERx));
	assert_param(IS_DTIM_CLOCK_SEQUENCE(DTIM));
		
	if(DTIM == DTIM_DUALTIMERx_1)
	{
		value = DUALTIMERx->Timer1Value;
	}
	else if(DTIM == DTIM_DUALTIMERx_2)
	{
		value = DUALTIMERx->Timer2Value;
	}
	return value;
}

/**
  *
  * @brief: 获取DualTimer原始中断状态。
  *
  * @param: DUALTIMERx,设置为STAR_DUALTIMER0或STAR_DUALTIMER1。
  * @param: DTIM,双定时器下的哪个定时器,双定时器共有两个定时器可供选择,DTIM_DUALTIMERx_1或DTIM_DUALTIMERx_2。
  *
  * @retVal: RIS,获取到的原始中断状态。
  */
uint32_t DTIM_Timer0RISValue(DUALTIMER_TypeDef* DUALTIMERx, uint32_t DTIM)
{		
	uint32_t RIS = 0;
	assert_param(IS_DUALTIMER_ALL_PERIPH(DUALTIMERx));
	assert_param(IS_DTIM_CLOCK_SEQUENCE(DTIM));
	
	if(DTIM == DTIM_DUALTIMERx_1)
	{
		RIS = DUALTIMERx->Timer1RIS;	
	}
	else if (DTIM == DTIM_DUALTIMERx_2)
	{
		RIS = DUALTIMERx->Timer2RIS;
	}
	return RIS;
}

/**
  *
  * @brief: 获取DualTimer中断寄存器状态。
  *
  * @param: DUALTIMERx,设置为STAR_DUALTIMER0或STAR_DUALTIMER1。
  * @param: DTIM,双定时器下的哪个定时器,双定时器共有两个定时器可供选择，DTIM_DUALTIMERx_1或DTIM_DUALTIMERx_2。
  *
  * @retVal: MIS,获取到的中断寄存器状态。
  */
uint32_t DTIM_Timer0MISValue(DUALTIMER_TypeDef* DUALTIMERx, uint32_t DTIM)
{		
	uint32_t MIS = 0;
	assert_param(IS_DUALTIMER_ALL_PERIPH(DUALTIMERx));
	assert_param(IS_DTIM_CLOCK_SEQUENCE(DTIM));
	
	if(DTIM == DTIM_DUALTIMERx_1)
	{
		MIS = DUALTIMERx->Timer1MIS;	

	}
	else if (DTIM == DTIM_DUALTIMERx_2)
	{
		MIS = DUALTIMERx->Timer2MIS;
	}
	return MIS;
}

