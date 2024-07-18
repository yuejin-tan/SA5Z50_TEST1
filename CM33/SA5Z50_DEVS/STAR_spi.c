#include "STAR_rcc.h"
#include "STAR_spi.h"
#include "STAR.h"
#include "STAR_gpio.h"



/**
  *
  * @brief: 将SPI寄存器值初始化为默认配置。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: void
  */
void SSP_DeInit(SSP_TypeDef* SSPx)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	
	if (SSPx == STAR_SSP0)
	{
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI0, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI0, DISABLE);
	}
	else if(SSPx == STAR_SSP1)
	{
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI1, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI1, DISABLE);
	}
}

/**
  *
  * @brief: SPI初始化配置。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * @param: SSP_InitStruct,SPI设置参数结构体变量，该结构体的成员变量包括传输数据的大小、SPI 工作模式、
  *								时钟分频系数、传输方式（分时或同时）、数据宽度、为回环模式或正常传输。
  * 
  * @retVal: void
  */
void SSP_Init(SSP_TypeDef* SSPx, SSP_InitTypeDef *SSP_InitStruct)
{	
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	assert_param(IS_SSP_Data_Size(SSP_InitStruct->Data_Size));
	assert_param(IS_SSP_SPHSPO(SSP_InitStruct->Mode));
	assert_param(IS_SSP_SCR_divisor(SSP_InitStruct->SCR));
	assert_param(IS_SSP_Prescale_divisor(SSP_InitStruct->CLK));
	assert_param(IS_SSP_TXRXSIMULT_Format(SSP_InitStruct->TXRXSIMULT));
	assert_param(IS_SSP_LBM_Work(SSP_InitStruct->LBM));	
	assert_param(IS_SSP_Data_Width(SSP_InitStruct->Width));	
	 
	SSPx->SSPCR0  = (0x000F & (SSP_InitStruct->Data_Size))|(SSP_InitStruct->Mode) | (SSP_InitStruct->SCR); 
	SSPx->SSPCPSR = (SSP_InitStruct->CLK &0x0ff);
	SSPx->SSPCR1  = (0x0000)|(SSP_InitStruct->TXRXSIMULT)|(SSP_InitStruct->LBM)|(SSP_InitStruct -> Width);
}

/**
  *
  * @brief: 使能SPI，让SPI开始工作。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * @param: NewState,是否使能SPI，ENABLE - 使能； DISABLE - 不使能。
  * 
  * @retVal: void
  */
void SSP_Enable(SSP_TypeDef* SSPx, FunctionalState NewState)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
		
	if (NewState != DISABLE)
    {
        SSPx->SSPCR1 |= (0x01UL << 1);	
    }
	else
	{
		SSPx->SSPCR1 &= ~(0x01UL << 1);
	}
}

/**
  *
  * @brief: SPI接收数据个数设置。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * @param: RxNum,需要接收数据的个数。
  * 
  * @retVal: void
  */
void SSP_Set_ReceiveDataNum(SSP_TypeDef* SSPx, uint32_t RxNum)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	
	SSPx->SSP_RX_CNT_LSB =  (RxNum&0xffff);
	SSPx->SSP_RX_CNT_MSB =  (RxNum>>16);
}

/**
  *
  * @brief: SPI中断设置。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * @param: SSP_IT,SPI中断类型，可设置的值包括：
  *					SSP_IT_RORIM - 接收溢出中断； SSP_IT_RTIM - 接收超时中断；
  *					SSP_IT_RXIM - 接收FIFO中断(接收FIFO半满或更少条件中断)；	
  *					SSP_IT_TXIM - 发送FIFO中断(发送FIFO半空或更少条件中断)；
  *					SSP_IT_RXFIM - 接收FIFO几乎满条件中断。
  * @param: NewState,是否使能中断；ENABLE - 使能中断；DISABLE - 不使能中断。
  * 
  * @retVal: void
  */
void SSP_Set_ITConfig(SSP_TypeDef* SSPx, uint16_t SSP_IT, FunctionalState NewState)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	assert_param(IS_SSP_CONFIG_IT(SSP_IT));
	
	if (NewState != DISABLE)
    {
		SSPx->SSPIMSC |= SSP_IT;	
    }
    else
    {
        SSPx->SSPIMSC &= ~SSP_IT;
    }
}

/**
  *
  * @brief: 使用SPI发送数据。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * @param: Data,需要发送的数据。
  * 
  * @retVal: void
  */
void SSP_SendData(SSP_TypeDef* SSPx, uint16_t Data)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	
	SSPx->SSPDR = Data;
}

/**
  *
  * @brief: 等待SPI数据发送完成。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: void
  */
void SSP_Wait_SendFinish(SSP_TypeDef* SSPx)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));	
	
    while(SSPx->SSPSR & (SSP_SR_BSY_TX_Msk));
}

/**
  *
  * @brief: 等待SPI接收数据，直到接收FIFO非空。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: void
  */
void SSP_Wait_ReceiveReady(SSP_TypeDef* SSPx)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	
	while(!(SSPx->SSPSR & SSP_SR_RNE_Msk));
}

/**
  *
  * @brief: 设置外设SPI的DMA请求。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * @param: TX_EN,SPI发送DMA使能；可设置的值包括：1- 使能；0- 不使能。
  * @param: RX_EN,SPI接收DMA使能；可设置的值包括：1- 使能；0- 不使能。
  *	
  * @retVal: void
  */
void SSP_DMA_Request(SSP_TypeDef* SSPx,int TX_EN,int RX_EN)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));

	SSPx->SSPDMACR = 0x00000000;
	SSPx->SSPDMACR |= (uint32_t)((TX_EN << 1)|(RX_EN << 0));
}

/**
  *
  * @brief: SPI接收数据。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: rx,接收到的数据。
  */
uint16_t SSP_ReceiveData(SSP_TypeDef* SSPx)
{	
	assert_param(IS_SSP_ALL_PERIPH(SSPx));

	return (uint16_t)SSPx->SSPDR;
	
}

/**
  *
  * @brief: 读取SPI状态寄存器SR当前值。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: 读取到的状态寄存器值。
  */
uint16_t SSP_Get_SRValue(SSP_TypeDef* SSPx)
{	
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	
	return (uint16_t)SSPx->SSPSR;
	
}

/**
  *
  * @brief: 读取SPI RIS寄存器当前值。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: RISValue,读取到的状态寄存器值。
  */
uint16_t SSP_Get_RISValue(SSP_TypeDef* SSPx)
{	
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	
	return (uint16_t)SSPx->SSPRIS;
	
}

/**
  *
  * @brief: 读取SPI MIS寄存器当前值。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: MISValue,读取到的状态寄存器值。
  */
uint16_t SSP_Get_MISValue(SSP_TypeDef* SSPx)
{	
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	
	return (uint16_t)SSPx->SSPMIS;
	
}

/**
  *
  * @brief: 清除SPI中断。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * @param: IT,中断类型，可设置的值包括：
  *						SSP_IT_RORIC - 清除SSPRORINTR(SPI接收溢出)中断；SSP_IT_RTIC - 清除SSPRTINTR(SPI接收超时)中断。
  * 
  * @retVal: void
  */
void SSP_ClearIT(SSP_TypeDef* SSPx, uint16_t IT)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	assert_param(IS_SSP_Clear_IT(IT) );
	if(IT == SSP_IT_RORIC)
	{
		SSPx->SSPICR |= SSP_IT_RORIC;
	}
	else if(IT == SSP_IT_RTIC)
	{
		SSPx->SSPICR |=  SSP_IT_RTIC;
	}
	
}

/**
  *
  * @brief: 等待直到SPI发送FIFO为空。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: void
  */
void SSP_Wait_TxFIFOEmpty (SSP_TypeDef* SSPx)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	while(!(SSPx->SSPSR & SSP_SR_TFE_Msk));
}

/**
  *
  * @brief: 等待直到SPI发送FIFO非满，可以继续发送数据。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: void
  */
void SSP_Wait_TxFIFONotFull(SSP_TypeDef* SSPx)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	while(!(SSPx->SSPSR & SSP_SR_TNF_Msk));
}

/**
  *
  * @brief: 等待直到SPI接收FIFO为空。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: void
  */
void SSP_Wait_RxFIFOEmpty (SSP_TypeDef* SSPx)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	while((SSPx->SSPSR & SSP_SR_RNE_Msk));
}

/**
  *
  * @brief: 等待直到SPI为空闲状态。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: void
  */
void SSP_Wait_PrimeCellIdle (SSP_TypeDef* SSPx)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	while(SSPx->SSPSR & SSP_SR_BSY_Msk);
}

/**
  *
  * @brief: 等待直到SPI发送为空闲状态，以保证发送完成。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: void
  */
void SSP_Wait_TxIdle (SSP_TypeDef* SSPx)
{
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	while(SSPx->SSPSR & SSP_SR_BSY_TX_Msk);
}

/**
  *
  * @brief: 发送FIFO是否已满。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: isFull,0-发送FIFO未满;1-发送FIFO已满。
  */
uint8_t SSP_Tx_FIFO_Is_Full(SSP_TypeDef* SSPx)
{
	uint8_t isFull=0x0;
	
	isFull = !(SSPx->SSPSR & SSP_SR_TNF_Msk);
	
	return isFull;
}


/**
  *
  * @brief: 接收FIFO是否为空。
  *
  * @param: SSPx,使用哪组SPI；STAR有2组SPI:STAR_SSP0和STAR_SSP1。
  * 
  * @retVal: isEmpty,0-接收FIFO不为空;1-接收FIFO为空。
  */
uint8_t SSP_RX_FIFO_Is_Empty(SSP_TypeDef* SSPx)
{
	uint8_t isEmpty=0x0;
	
	isEmpty=!(SSPx->SSPSR & SSP_SR_RNE_Msk); 
	
	
	return isEmpty;
}

