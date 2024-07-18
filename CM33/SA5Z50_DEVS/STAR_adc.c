#include "STAR_adc.h"
#include "STAR_rcc.h"

#include "stdint.h"

/**
  *
  * @brief: 将ADC寄存器初始化为默认配置。
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  * 
  * @retVal: void
  */
void ADC_DeInit(ADC_TypeDef* ADCx)
{
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	
	if (ADCx == STAR_ADC0)
	{
		RCC_APB1PeriphResetCmd( RCC_APB1Periph_ADC, ENABLE );
		RCC_APB1PeriphResetCmd( RCC_APB1Periph_ADC, DISABLE );
	} 
	else if (ADCx == STAR_ADC1)
	{
		RCC_APB1PeriphResetCmd( RCC_APB1Periph_ADC1, ENABLE );
		RCC_APB1PeriphResetCmd( RCC_APB1Periph_ADC1, DISABLE );
	} 
}


/**
  *
  * @brief: ADC初始化设置。
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  * @param: ADC_InitStruct,ADC设置参数结构体变量，该结构体变量包括时钟分频系数、转换模式（连续或单次）、
  *							上电、数据缓冲、溢出标志、外部触发、触发类型、数据对齐方式、扫描模式。
  * 
  * @retVal: void
  */
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_CHANNEL(ADC_InitStruct->Channel));
	assert_param(IS_ADC_REF_VOL_SEL(ADC_InitStruct->RefVol_sel));
	assert_param(IS_ADC_ACTIVE_STATE(ADC_InitStruct->ADC_PDn));
	assert_param(IS_ADC_Clk_PreDiv(ADC_InitStruct->Clk_Prescaler));
	assert_param(IS_ADC_TRANSFOR_MODE(ADC_InitStruct->Transfor_mode));
	assert_param(IS_ADC_Converter_ADON(ADC_InitStruct->Converter_adon));
	assert_param(IS_ADC_External_TrigEnable(ADC_InitStruct->Ext_trig)); 
	assert_param(IS_ADC_DATA_ALIGN(ADC_InitStruct->Data_align)); 
	assert_param(IS_ADC_EXT_EVENT_SEL(ADC_InitStruct->Ext_sel));
	assert_param(IS_ADC_Scan_Mode(ADC_InitStruct->ScanConvMode));
	assert_param(IS_ADC_Data_buffer(ADC_InitStruct->Data_buffer));
	assert_param(IS_ADC_Overrun_flag(ADC_InitStruct->Overrun_flag));
	
	ADCx->ADC_CSR_1 = (uint32_t)(((ADC_InitStruct->Channel)<<0)|((ADC_InitStruct->RefVol_sel)<<4)
															 |((ADC_InitStruct->ADC_PDn)<<5))	;
	 

	ADCx->ADC_CR_1 = (uint32_t)(((ADC_InitStruct->Clk_Prescaler)<<4) |((ADC_InitStruct->Transfor_mode)<<1) 
																|((ADC_InitStruct->Converter_adon)<<0));
	 
	ADCx->ADC_CR_2 = (uint32_t)((((ADC_InitStruct->Data_buffer )<<17) |(( ADC_InitStruct->Overrun_flag)<<16) 
									|(( ADC_InitStruct->Ext_trig)<<6)|(( ADC_InitStruct->Ext_sel)<<4)
									|(( ADC_InitStruct->Data_align)<<3)|(( ADC_InitStruct->ScanConvMode)<<1))); 
}


/**
  *
  * @brief: 启动ADC转换。
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  * 
  * @retVal: void
  */
void ADC_StartConversion(ADC_TypeDef* ADCx) 
{
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	
	ADCx->ADC_CR_1 |= (1<<0);//ADON Write 1
}


/**
  *
  * @brief: 停止ADC转换。
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  * 
  * @retVal: void
  */
void ADC_StopConversion(ADC_TypeDef* ADCx)
{	
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	
	ADCx->ADC_CR_1 &= ~(1U<<0);//ADON Write 0
	
}

/**
  *
  * @brief: ADC EOC中断配置，在ADC转换结束时是否产生中断。
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  * @param: NewState,是否使能中断，ENABLE - 使能中断；DISABLE - 不使能中断。
  * 
  * @retVal: void
  */
void ADC_EOCInterrupt_Config(ADC_TypeDef* ADCx,FunctionalState NewState)
{
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
  	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if (NewState != DISABLE)
	{ 
		ADCx->ADC_CSR_0 |= (0x1<<1);
	}
	else
	{ 	
		ADCx->ADC_CSR_0 &= ~(0x1U<<1);
	}
}


/**
  *
  * @brief: 读取ADC采样数据(单次或非缓冲连续模式时),单次或非缓冲连续模式下，ADC转换的值存储在ADC_DR寄存器中。
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  * 
  * @retVal: 读取到的ADC采样数据。
  */
uint16_t ADC_GetValueFromDR(ADC_TypeDef* ADCx)
{	
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	
	return ADCx->ADC_DR&0x0ffff;	
}

/**
  *
  * @brief: 读取ADC采样数据,在开启数据缓冲模式或扫描模式时，ADC 转换的值存储在ADC_DBR_x 寄存器中。
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  * @param: ADC_buf,存储ADC采样数据的缓存。
  * 
  * @retVal: void
  */
void ADC_GetValueFromDBR(ADC_TypeDef* ADCx, uint32_t* ADC_buf)
{	
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	
	
	if(ADC_buf ==NULL)
	{
		return;
	}
	ADC_buf[0] = ADCx->ADC_DBR_1_0;	//ADC_DBR0:bit0~bit15;	ADC_DBR1:bit16~bit31
	ADC_buf[1] = ADCx->ADC_DBR_3_2;	//ADC_DBR2:bit0~bit15;	ADC_DBR3:bit16~bit31 
	ADC_buf[2] = ADCx->ADC_DBR_5_4;	//ADC_DBR4:bit0~bit15;	ADC_DBR5:bit16~bit31
	ADC_buf[3] = ADCx->ADC_DBR_7_6;	//ADC_DBR6:bit0~bit15;	ADC_DBR7:bit16~bit31
	ADC_buf[4] = ADCx->ADC_DBR_9_8;	//ADC_DBR8:bit0~bit15;	ADC_DBR9:bit16~bit31
	ADC_buf[5] = ADCx->ADC_DBR_11_10;	//ADC_DBR10:bit0~bit15;	ADC_DBR11:bit16~bit31 
	ADC_buf[6] = ADCx->ADC_DBR_13_12;	//ADC_DBR12:bit0~bit15;	ADC_DBR13:bit16~bit31
	ADC_buf[7] = ADCx->ADC_DBR_15_14;	//ADC_DBR14:bit0~bit15;	ADC_DBR15:bit16~bit31
	
}

/**
  *
  * @brief: 外部触发ADC启动设置。
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  * @param: ADC_ExternEvent,设置的外部触发事件；
  * 							可设置的值包括：EXT_EVENT_SEL_Timer0(0x0) - 内部TRGO事件，连接到定时器0中断；
  *												EXT_EVENT_SEL_Pin (0x01) - ADC_ETR管脚上的中断。	
  * 
  * @retVal: void
  */
void ADC_ExternalEventTrigConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternEvent)
{	
	uint32_t temp;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_EXT_EVENT_SEL(ADC_ExternEvent));
	
	
	temp = ADCx->ADC_CR_2;
	temp &= ~(0x03U<<4);
	temp |= (ADC_ExternEvent<<4);
	ADCx->ADC_CR_2 = temp;
	
}

/**
  *
  * @brief: 清除ADC转换结束(EOC)中断标志。
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  * 
  * @retVal: void
  */
void ADC_InterruptEOCFlag_Clear(ADC_TypeDef* ADCx)
{
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	
	ADCx->ADC_CSR_0 &= ~(1U<<0);
}

/**
  *
  * @brief: 获取ADC busy状态
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  * 
  * @retVal: state,获取到的ADC busy状态，1-ADC busy状态；0- ADC非busy状态。
  */
uint8_t ADC_Get_BusyState(ADC_TypeDef* ADCx)
{
	uint8_t state;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	
	state =(uint8_t)(ADCx->ADC_CSR_1 & 0x100U);
	
	return state;
}

/**
  *
  * @brief: 设置ADC的激活或PowerDown状态
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  *			ADC_ActiveState,ADC的激活或PowerDown状态，可设置为激活(ADC_ACTIVE)或PowerDown(ADC_INACTIVE_POWER_DOWN)。
  * 
  * @retVal: void
  */
void ADC_Active_Set(ADC_TypeDef* ADCx, uint8_t ADC_ActiveState)
{
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_ACTIVE_STATE(ADC_ActiveState));
	
	if(ADC_ActiveState ==ADC_ACTIVE)
	{
		ADCx->ADC_CSR_1 |= (0x01<<5);
	}
	else
	{
		ADCx->ADC_CSR_1 &= ~(0x01U<<5);
	}
}

/**
  *
  * @brief: 获取ADC Data buffer使能状态
  *
  * @param: ADCx,使用哪组ADC,STAR有两组ADC,可设置为STAR_ADC0或STAR_ADC1。
  * 
  * @retVal: state,ADC Data buffer使能状态,1-Data buffer使能；0- Data buffer没使能。
  */
uint8_t ADC_Get_DataBufferEnable_State(ADC_TypeDef* ADCx)
{
	uint8_t state=0;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	
	if(ADCx->ADC_CR_2 & (1<<17))
	{
		state= 1;
	}
	else
	{
		state= 0;
	}
	return state;
	
}



