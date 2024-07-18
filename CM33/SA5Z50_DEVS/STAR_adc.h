#ifndef __STAR_ADC_H
#define __STAR_ADC_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "STAR.h"


#ifndef NULL
#define NULL 0
#endif

/** 
  * @brief  ADC Init structure definition  
  */

typedef struct
{	
	uint8_t Channel;
	uint8_t RefVol_sel;
	uint8_t ADC_PDn;
	uint8_t Clk_Prescaler;																									 
	uint8_t Transfor_mode;
	uint8_t Converter_adon;		 
	uint8_t Data_buffer;
	uint8_t Overrun_flag;
	uint8_t Ext_trig;    
	uint8_t Ext_sel;
	uint8_t Data_align;
	FunctionalState ScanConvMode; 
}ADC_InitTypeDef;

/**
  * @}
  */

#define Single_Conversion                       (0x0)
#define Continuous_Conversion                  	(0x1)
#define IS_ADC_TRANSFOR_MODE(MODE) (((MODE) == Single_Conversion) || (( MODE) == Continuous_Conversion))

#define Power_down                       				(0x0)
#define Power_on                  							(0x1)
#define IS_ADC_Converter_ADON(ADON) (((ADON) == Power_down) || (( ADON) == Power_on))



/** @defgroup ADC_Exported_Constants
  * @{
  */

#define IS_ADC_ALL_PERIPH(PERIPH) (((PERIPH) == STAR_ADC0)||((PERIPH) == STAR_ADC1))

/** @defgroup ADC_frequency 
  * @{
  */
#define ADC_Clk_PreDiv_2					(0x0)	
#define ADC_Clk_PreDiv_4					(0x1)		
#define ADC_Clk_PreDiv_6					(0x2)		
#define ADC_Clk_PreDiv_8					(0x3)		
#define ADC_Clk_PreDiv_10					(0x4)		
#define ADC_Clk_PreDiv_12					(0x5)		
#define ADC_Clk_PreDiv_24					(0x6)		
#define ADC_Clk_PreDiv_48					(0x7)		

#define IS_ADC_Clk_PreDiv(Clk)    (((Clk) == ADC_Clk_PreDiv_2)  || ((Clk) == ADC_Clk_PreDiv_4) || \
                                 ((Clk) == ADC_Clk_PreDiv_6)  || ((Clk) == ADC_Clk_PreDiv_8) || \
                                 ((Clk) == ADC_Clk_PreDiv_10) || ((Clk) == ADC_Clk_PreDiv_12)|| \
                                 ((Clk) == ADC_Clk_PreDiv_24) || ((Clk) == ADC_Clk_PreDiv_48))


/**
  * @}
  */

#define DBUF_Disable                  				   (0x0)
#define DBUF_Enable                    				   (0x1)	
#define IS_ADC_Data_buffer(buffer) (((buffer) == DBUF_Enable) || \
																	((buffer) == DBUF_Disable))


#define NO_OVR											(0x0)																		
#define EN_OVR                    				(0x1)
#define IS_ADC_Overrun_flag(flag) (((flag) == NO_OVR) || \
																	 ((flag) == EN_OVR))
/** @defgroup ADC_data_align 
  * @{
  */

#define DataAlign_Right                          (0x0)
#define DataAlign_Left                           (0x1)
#define IS_ADC_DATA_ALIGN(ALIGN) (((ALIGN) == DataAlign_Right) || \
																	((ALIGN) == DataAlign_Left))
/**
  * @}
  */
	
/** @defgroup ADC_ScanConv_Mode
  * @{
  */
	

#define ScanMode_Disable												 (0x0)
#define	ScanMode_Enable 												 (0x1)
#define	IS_ADC_Scan_Mode(Mode) (((Mode) == ScanMode_Enable) || \
																	(Mode) == ScanMode_Disable))
	
/** @defgroup ADC_ExternalTrigConv
  * @{
  */ 
#define External_Trig_Enable													 (0x1)
#define External_Trig_Disable												 (0x0)

#define	IS_ADC_External_TrigEnable(TrigConv) (((TrigConv) == External_Trig_Enable) || \
																				((TrigConv) == External_Trig_Disable))
											
/** @defgroup ADC_interrupts_definition 
  * @{
  */
#define ADC_IT_EOCIE_Enable 												((uint32_t)0x00000002)
#define ADC_IT_EOCIE_Disable												((uint32_t)0x00000000)
//#define IS_ADC_IT(IT) ((IT) == ADC_IT_EOCIE)


#define ADC_START_Enable 														 ((uint32_t)0x00000001)
#define ADC_START_Disable														 ((uint32_t)0x00000000)


#define EXT_EVENT_SEL_Timer0 													 (0x0)
#define EXT_EVENT_SEL_Pin															 (0x1)
#define EXT_EVENT_SEL_Reserved_0												 (0x2)
#define EXT_EVENT_SEL_Reserved_1												 (0x3)
#define	IS_ADC_EXT_EVENT_SEL(INJTRIG) (((INJTRIG) == EXT_EVENT_SEL_Timer0)     || \
																((INJTRIG) == EXT_EVENT_SEL_Pin)			|| \
																((INJTRIG) == EXT_EVENT_SEL_Reserved_0) 	|| \
																((INJTRIG) == EXT_EVENT_SEL_Reserved_1))


#define ADC_INACTIVE_POWER_DOWN				(0x0)
#define ADC_ACTIVE							(0x1)
#define	IS_ADC_ACTIVE_STATE(ActiveState) (((ActiveState) == ADC_INACTIVE_POWER_DOWN) || \
																				((ActiveState) == ADC_ACTIVE))
																				
#define INTERIOR_REF_VOL			(0x0)
#define EXTERNAL_REF_VOL			(0x1)																			
#define IS_ADC_REF_VOL_SEL(ADC_RefVol)		(((ADC_RefVol) == INTERIOR_REF_VOL) || \
																					((ADC_RefVol) == EXTERNAL_REF_VOL))
/**
  * @}
  */


/** @defgroup ADC_channels 
  * @{
  */

#define Channel_0                                (0x0)		// Available ADC_IN0
#define Channel_1                                (0x1)   // Available ADC_IN1
#define Channel_2                                (0x2)   // Available ADC_IN2
#define Channel_3                                (0x3)   // Available ADC_IN3
#define Channel_4                                (0x4)   // Available ADC_IN4
#define Channel_5                                (0x5)   // Available ADC_IN5
#define Channel_6                                (0x6)   // Available ADC_IN6
#define Channel_7                                (0x7)   // Available ADC_IN7

//Channel_8 ~ Channel_11 are only useful for ADC0.
#define Channel_8                                (0x8)   // Available ADC_IN8
#define Channel_9                                (0x9)   // Available ADC_IN9
#define Channel_10                               (0x10)  // Available ADC_IN10
#define Channel_11                               (0x11)  // Available ADC_IN11



#define IS_ADC_CHANNEL(CHANNEL) 	 (((CHANNEL) == _Channel_0) || ((CHANNEL) == _Channel_1) \
																		((CHANNEL) == _Channel_2) || \
																		((CHANNEL) == _Channel_3) || \
																		((CHANNEL) == _Channel_4) || \
																		((CHANNEL) == _Channel_5) || \
																		((CHANNEL) == _Channel_6) || \
																		((CHANNEL) == _Channel_7)) || \
																		((CHANNEL) == _Channel_8)) || \
																		((CHANNEL) == _Channel_9)) || \
																		((CHANNEL) == _Channel_10))|| \
																		((CHANNEL) == _Channel_11))
/**
  * @}
  */

/** @defgroup ADC_Exported_Functions
  * @{
  */
 
void ADC_DeInit(ADC_TypeDef* ADCx);
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StartConversion(ADC_TypeDef* ADCx);
void ADC_StopConversion(ADC_TypeDef* ADCx);
void ADC_EOCInterrupt_Config(ADC_TypeDef* ADCx,FunctionalState NewState);
uint16_t ADC_GetValueFromDR(ADC_TypeDef* ADCx);
void ADC_GetValueFromDBR(ADC_TypeDef* ADCx, uint32_t* ADC_buf);
void ADC_ExternalEventTrigConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternEvent);
void ADC_InterruptEOCFlag_Clear(ADC_TypeDef* ADCx);
uint8_t ADC_Get_BusyState(ADC_TypeDef* ADCx);
void ADC_Active_Set(ADC_TypeDef* ADCx, uint8_t ADC_ActiveState);
uint8_t ADC_Get_DataBufferEnable_State(ADC_TypeDef* ADCx);

#ifdef __cplusplus
}
#endif

#endif /* __STAR_ADC_H */

