
#ifndef __STAR_SPI_H
#define __STAR_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "STAR.h"


typedef struct
{
	uint16_t	Data_Size;  		//数据的位数 4-16位可选
	uint16_t	CLK;				//SPI时钟预分频系数
	uint16_t	SCR;				//Serial clock rate;决定SPI时钟速率的因子系数 
	uint16_t	Mode;				//SPI的四种工作模式
	uint16_t  	TXRXSIMULT; 		//同时收发只适用于回环模式；分时收发适用于单独的传输或接收
	uint16_t  	LBM;				//回环模式；或正常串口运行
	uint16_t  	Width;				//data width：standard  dual  quad	
		
}SSP_InitTypeDef; 

#define IS_SSP_ALL_PERIPH(PERIPH) (((PERIPH) == STAR_SSP0)||((PERIPH) == STAR_SSP1);	


#define SSP_Data_Size_4bit              							((uint8_t)0x03)
#define SSP_Data_Size_5bit              		  					((uint8_t)0x04)
#define SSP_Data_Size_6bit         					 				((uint8_t)0x05)
#define SSP_Data_Size_7bit           								((uint8_t)0x06)
#define SSP_Data_Size_8bit           								((uint8_t)0x07)
#define SSP_Data_Size_9bit           								((uint8_t)0x08)
#define SSP_Data_Size_10bit           								((uint8_t)0x09)
#define SSP_Data_Size_11bit           								((uint8_t)0x0a)
#define SSP_Data_Size_12bit           								((uint8_t)0x0b)
#define SSP_Data_Size_13bit           								((uint8_t)0x0c)
#define SSP_Data_Size_14bit           								((uint8_t)0x0d)
#define SSP_Data_Size_15bit           								((uint8_t)0x0e)
#define SSP_Data_Size_16bit           								((uint8_t)0x0f)
#define IS_SSP_Data_Size(Size) 			( ((Size) ==  SSP_Data_Size_4bit )|| \
																			((Size) ==  SSP_Data_Size_5bit) || \
																			((Size) ==  SSP_Data_Size_6bit) || \
																			((Size) ==  SSP_Data_Size_7bit) || \
																			((Size) ==  SSP_Data_Size_8bit) || \
                                      ((Size) ==  SSP_Data_Size_9bit) || \
																			((Size) ==  SSP_Data_Size_10bit) || \
																			((Size) ==  SSP_Data_Size_11bit) || \
																			((Size) ==  SSP_Data_Size_12bit) || \
																			((Size) ==  SSP_Data_Size_13bit) || \
																			((Size) ==  SSP_Data_Size_14bit) || \
																			((Size) ==  SSP_Data_Size_15bit) || \
																			((Size) ==  SSP_Data_Size_16bit))
	
// The four working modes of SPI	
#define SSP_SPH0SPO0              										((uint8_t)0x00)	
#define SSP_SPH1SPO0              										((uint8_t)0x80)	
#define SSP_SPH0SPO1              										((uint8_t)0x40)	
#define SSP_SPH1SPO1              										((uint8_t)0xC0)	
#define IS_SSP_SPHSPO_Mode(Mode) 					( ((Mode) ==  SSP_SPH0SPO0) || \
																						((Mode) ==  SSP_SPH1SPO0) || \
																						((Mode) ==  SSP_SPH0SPO1) || \
																						((Mode) ==  SSP_SPH1SPO1))

// Time-sharing or simultaneous transmission
#define SSP_TXRXSIMULT_Time_Sharing              			(0x0UL<<6)								
#define SSP_TXRXSIMULT_simultaneous           				(0x1UL<<6)	
#define IS_SSP_TXRXSIMULT_Format(Format)		(((Format) ==  SSP_TXRXSIMULT_Time_Sharing) || \
																						 ((Format) ==  SSP_TXRXSIMULT_simultaneous))

#define SSP_Width_Standard              							(0x0UL<<4)								
#define SSP_Width_Dual           											(0x1UL<<4)	
#define SSP_Width_Quad 																(0x2UL<<4)
#define IS_SSP_Data_Width(Width)							( ((Width) ==  SSP_Width_Standard) || \
																								((Width) ==  SSP_Width_Dual) || \
																								((Width) ==  SSP_Width_Quad))	

// Select the first one if loopback transport mode is used; Otherwise choose the second one;							
#define SSP_LBM_Normal              									(0x0UL<<0)								
#define SSP_LBM_LoopBack           										(0x1UL<<0)	
#define IS_SSP_LBM_Work(Work)							( ((Work) ==  SSP_LBM_Normal) || \
																						((Work) ==  SSP_LBM_LoopBack))							
										


#define SSP_CLK_250Prescale									((uint16_t)0x00fa)	 
#define SSP_CLK_200Prescale									((uint16_t)0x00c8)	 
#define SSP_CLK_100Prescale									((uint16_t)0x0064)	 
#define SSP_CLK_50Prescale									((uint16_t)0x0032)   
#define SSP_CLK_20Prescale									((uint16_t)0x0014)   
#define SSP_CLK_12Prescale									((uint16_t)0x000c)   
#define SSP_CLK_10Prescale								  	((uint16_t)0x000a)
#define SSP_CLK_8Prescale								  	((uint16_t)0x0008)
#define SSP_CLK_6Prescale								  	((uint16_t)0x0006)   
#define SSP_CLK_4Prescale									((uint16_t)0x0004)	 
#define SSP_CLK_2Prescale								    ((uint16_t)0x0002)

#define SSP_SCR_0              										((uint16_t)0x0000)
#define SSP_SCR_2              										((uint16_t)0x0200)		
#define SSP_SCR_4              										((uint16_t)0x0400)	
#define SSP_SCR_8              										((uint16_t)0x0500)	
#define SSP_SCR_10              									((uint16_t)0x0a00)	
#define SSP_SCR_16              									((uint16_t)0x1000)	
#define SSP_SCR_32              									((uint16_t)0x2000)	


#define SSP_IT_RXFIM 								 ((uint16_t)0x0010)
#define SSP_IT_TXIM 								 ((uint16_t)0x0008)
#define SSP_IT_RXIM 								 ((uint16_t)0x0004)
#define SSP_IT_RTIM 								 ((uint16_t)0x0002)
#define SSP_IT_RORIM 								 ((uint16_t)0x0001)
#define IS_SSP_CONFIG_IT(SSP_IT) 			( ((SSP_IT) == SSP_IT_RXFIM) || \
	                                			((SSP_IT) == SSP_IT_TXIM)  || \
	                                			((SSP_IT) == SSP_IT_RXIM)  || \
	                                			((SSP_IT) == SSP_IT_RTIM)  || \
	                                			((SSP_IT) == SSP_IT_RORIM))

#define SSP_IT_RORIC 								 ((uint16_t)0x0001)
#define SSP_IT_RTIC 								 ((uint16_t)0x0002)
#define IS_SSP_Clear_IT(IT) 				( ((IT) == SSP_IT_RORIC) || \
																			((IT) == SSP_IT_RTIC))
/** @defgroup SSP_Exported_Functions
  * @{
  */

void SSP_DeInit(SSP_TypeDef* SSPx);
void SSP_Init(SSP_TypeDef* SSPx, SSP_InitTypeDef *SSP_InitStruct);
void SSP_Enable(SSP_TypeDef* SSPx, FunctionalState NewState);
void SSP_Set_ReceiveDataNum(SSP_TypeDef* SSPx, uint32_t RxNum);
void SSP_Set_ITConfig(SSP_TypeDef* SSPx, uint16_t SSP_IT, FunctionalState NewState);
void SSP_SendData(SSP_TypeDef* SSPx, uint16_t Data);
void SSP_Wait_SendFinish(SSP_TypeDef* SSPx);
void SSP_Wait_ReceiveReady(SSP_TypeDef* SSPx);
void SSP_DMA_Request(SSP_TypeDef* SSPx,int TX_EN,int RX_EN);
uint16_t SSP_ReceiveData(SSP_TypeDef* SSPx);
uint16_t SSP_Get_SRValue(SSP_TypeDef* SSPx);
uint16_t SSP_Get_RISValue(SSP_TypeDef* SSPx);
uint16_t SSP_Get_MISValue(SSP_TypeDef* SSPx);
void SSP_ClearIT(SSP_TypeDef* SSPx, uint16_t IT);
void SSP_Wait_TxFIFOEmpty (SSP_TypeDef* SSPx);
void SSP_Wait_TxFIFONotFull(SSP_TypeDef* SSPx);
void SSP_Wait_RxFIFOEmpty (SSP_TypeDef* SSPx);
void SSP_Wait_PrimeCellIdle (SSP_TypeDef* SSPx);
void SSP_Wait_TxIdle (SSP_TypeDef* SSPx);
uint8_t SSP_Tx_FIFO_Is_Full(SSP_TypeDef* SSPx);
uint8_t SSP_RX_FIFO_Is_Empty(SSP_TypeDef* SSPx);
#ifdef __cplusplus
}
#endif

#endif /* __STAR_SPI_H */
															

