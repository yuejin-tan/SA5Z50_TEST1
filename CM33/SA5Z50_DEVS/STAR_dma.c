#include "STAR_rcc.h"
#include "STAR_gpio.h"
#include "STAR.h"	
#include "STAR_dma.h"	

#include "stdint.h"

/**
  *
  * @brief: 读取DMA中断状态寄存器。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * 
  * @retVal: IntStatus,读取到的中断状态寄存器值。
  */
uint32_t DMA_Get_InterruptStatus(DMA_TypeDef *DMAx)
{	
	
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	return DMAx->DMACIntStatus;
	
}

/**
  *
  * @brief: 读取DMA终端计数中断状态寄存器。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * 
  * @retVal: IntTCStatus,读取到的终端计数中断状态寄存器值。
  */
uint32_t DMA_Get_TerminalCountStatus(DMA_TypeDef *DMAx)
{		
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	return DMAx->DMACIntTCStatus;
}

/**
  *
  * @brief: 清除DMA终端计数中断。
  *
  * @param: CDMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: NewState,是否清除中断；ENABLE -清除；DISABLE -不清除。
  * 
  * @retVal: void
  */
void DMA_Clear_TerminalCountInterrupt(DMA_TypeDef *DMAx, uint8_t Channel, FunctionalState NewState)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	assert_param(IS_DMA_Channel(Channel));
		
	if(NewState != DISABLE)
	{
		DMAx->DMACIntTCClear |= (1<<Channel);
	}
	
}

/**
  *
  * @brief: 读取DMA错误中断状态寄存器。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * 
  * @retVal: IntErrorStatus,读取到的错误中断状态寄存器值。
  */
uint32_t DMA_Get_InterruptErrorStatus(DMA_TypeDef *DMAx)
{	
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	return DMAx->DMACIntErrorStatus;
}

 /**
  *
  * @brief: 清除DMA错误中断请求。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: NewState,是否清除；ENABLE -清除；DISABLE -不清除。
  * 
  * @retVal: void
  */
void DMA_Clear_InterruptError(DMA_TypeDef *DMAx, uint8_t Channel, FunctionalState NewState)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_Channel(Channel));
	
	if(NewState != DISABLE)
	{
		DMAx->DMACIntErrClr |= (1<<Channel);
	}
}

/**
  *
  * @brief: 读取DMA终端计数屏蔽之前的中断状态。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * 
  * @retVal: RawIntTCStatus,读取到中断状态。
  */
uint32_t DMA_Get_RawInterruptTerminalCountStatus(DMA_TypeDef *DMAx)
{		
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	return DMAx->DMACRawIntTCStatus;
	
}

/**
  *
  * @brief: 读取DMA屏蔽之前的错误中断状态。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * 
  * @retVal: RawIntErrorStatus,读取到中断状态。
  */
uint32_t DMA_Get_RawErrorInterruptStatus(DMA_TypeDef *DMAx)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	return DMAx->DMACRawIntTCStatus;
	
}

/**
  *
  * @brief: 读取DMA通道使能状态。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * 
  * @retVal: EnabledChannel,读取到的通道使能状态；bit0 -通道0通道使能状态；bit1 - 通道1通道使能状态。
  */
uint32_t DMA_Get_EnableChannel(DMA_TypeDef *DMAx)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	return DMAx->DMACEnbldChns;
}

/**
  *
  * @brief: DMA配置。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: M,AHB主端大小端格式；0- 小端格式；1- 大端格式；默认为0。
  * @param: E,DMA控制器使能；0 -不使能；1 -使能。
  * 
  * @retVal: void
  */
void DMA_Set_Configuration(DMA_TypeDef *DMAx, int M, int E)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	DMAx->DMACConfiguration = 0x00000000;
	
	DMAx->DMACConfiguration |= (uint32_t)((M<<1)|(E<<0));
}

/**
  *
  * @brief: 设置DMA源地址。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: addr,设置的源地址。
  * 
  * @retVal: void
  */
void DMA_Set_SourceAddress(DMA_TypeDef *DMAx, uint8_t Channel, uint32_t addr)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_Channel(Channel));
	
	if(Channel == Channel_0)
	{
		DMAx->DMACC0SrcAddr = 0x00000000;
		DMAx->DMACC0SrcAddr = addr;
	}
	else
	{
		DMAx->DMACC1SrcAddr = 0x00000000;
		DMAx->DMACC1SrcAddr = addr;
	}
}

/**
  *
  * @brief: 设置DMA目的地址。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: addr,设置的目的地址。
  * 
  * @retVal: void
  */
void DMA_Set_DestinationAddress(DMA_TypeDef *DMAx, uint8_t Channel, uint32_t addr)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_Channel(Channel));
	
	if(Channel == Channel_0)
	{
		DMAx->DMACC0DestAddr = 0x00000000;
		DMAx->DMACC0DestAddr = addr;
	}
	else
	{
		DMAx->DMACC1DestAddr = 0x00000000;
		DMAx->DMACC1DestAddr = addr;
	}
}

/**
  *
  * @brief: 设置DMA的链式传输。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: data,链表项，下一个链表项。
  * 
  * @retVal: void
  */
void DMA_Set_LLI(DMA_TypeDef *DMAx, uint8_t Channel, uint32_t data)
{		
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_Channel(Channel));
	
	if(Channel == Channel_0)
	{
		DMAx->DMACC0LLI = 0x00000000;
		DMAx->DMACC0LLI = data;
	}
	else
	{
		DMAx->DMACC1LLI = 0x00000000;
		DMAx->DMACC1LLI = data;
	}
}

/**
  *
  * @brief: 配置DMA的源地址和目标地址在每次传输后是否递增。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: I,终端计数中断使能；可设置的值包括：I_IT_Dis(0x0) - 不使能中断，I_IT_En (0x01) - 使能中断；默认不使能中断。
  * @param: DI,目的地址是否递增；可设置的值包括：DI_En(0x1) -递增；DI_Dis(0x0) - 不递增。
  * @param: SI,源地址是否递增；可设置的值包括：SI_En(0x1) -递增； SI_Dis (0x0) -不递增。
  *	
  * @retVal: void
  */
void DMA_Set_Increment(DMA_TypeDef *DMAx, uint8_t Channel, int I, int DI, int SI)								   
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_Channel(Channel));

	if(Channel== Channel_0)
	{
		DMAx->DMACC0Control &= ~((0x01U<<31)|(0x01U<<27)|(0x01U<<26));
		DMAx->DMACC0Control |= (uint32_t)((I<<31)|(DI<<27)|(SI<<26));
	}
	else
	{
		DMAx->DMACC1Control &= ~((0x01U<<31)|(0x01U<<27)|(0x01U<<26));
		DMAx->DMACC1Control |= (uint32_t)((I<<31)|(DI<<27)|(SI<<26));
	}
}

/**
  *
  * @brief: 设置DMA传输数据的数据宽度。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: DW,目标地址接收数据的宽度；可设置的值包括：
  *					SWorDWidth_8bit_Byte -8bit; SWorDWidth_16bit_Halfword - 16bit; SWorDWidth_32bit_Word - 32bit。
  * @param: SW,源地址接收数据的宽度；可设置的值包括：
  *					SWorDWidth_8bit_Byte -8bit; SWorDWidth_16bit_Halfword - 16bit; SWorDWidth_32bit_Word - 32bit。
  *	
  * @retVal: void
  */
void DMA_Set_DataWidth(DMA_TypeDef *DMAx,uint8_t Channel,int DW ,int SW)
{	
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_Channel(Channel));
	
	if(Channel==Channel_0)
	{
		DMAx->DMACC0Control &= ~((0x7U<<21)|(0x07U<<18));
		DMAx->DMACC0Control |= (((uint32_t)DW<<21)|((uint32_t)SW<<18));
	}
	else
	{
		DMAx->DMACC1Control &= ~((0x7U<<21)|(0x07U<<18));
		DMAx->DMACC1Control |= (((uint32_t)DW<<21)|((uint32_t)SW<<18));
	}
}

/**
  *
  * @brief: 设置DMA传输数据的个数。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: DB,目标地址突发传输的数据个数；可设置的值包括：
  *					DB_BurstSize_1 - 1个；DB_BurstSize_4 - 4个；DB_BurstSize_8 -8个；DB_BurstSize_16 - 16个；
  *					DB_BurstSize_32 - 32个； DB_BurstSize_64 - 64个； DB_BurstSize_128 -128个；DB_BurstSize_256 -256个。
  * @param: SB,源地址突发传输的数据个数；可设置的值包括：
  *					SB_BurstSize_1 -1个；SB_BurstSize_4 - 4个；SB_BurstSize_8 -8个；SB_BurstSize_16 - 16个；
  *					SB_BurstSize_32 - 1个；SB_BurstSize_64 -64个；SB_BurstSize_128 -128个；SB_BurstSize_256 -256个。
  * @param: Tx_Size,正常传输下数据的个数。
  *	
  * @retVal: void
  */
void DMA_Set_TransferSize(DMA_TypeDef *DMAx, uint8_t Channel, int DB, int SB, int Tx_Size)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_Channel(Channel));
	
	if(Channel ==Channel_0)
	{
		DMAx->DMACC0Control &= 0xfffc0000U;
		DMAx->DMACC0Control |= ((uint32_t)DB<<15)|((uint32_t)SB<<12)|((uint32_t)Tx_Size<<0);
	}
	else
	{
		DMAx->DMACC1Control &= 0xfffc0000U;
		DMAx->DMACC1Control |= ((uint32_t)DB<<15)|((uint32_t)SB<<12)|((uint32_t)Tx_Size<<0);
	}
}

/**
  *
  * @brief: DMA通道配置。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: Halt,是否忽略后续的源DMA请求；可设置的值包括：Halt_Dis(0x0) - 不忽略；Halt_En(0x01) - 忽略。
  * @param: Lock,是否锁定传输；可设置的值包括： Lock_Dis(0x0) - 不锁定；Lock_En(0x01) - 锁定。
  * @param: ITC,终端计数中断掩码；可设置的值包括：ITC_Mask(0x0) -屏蔽相关通道的终端计数中断;
  * 												ITC_NotMask(0x01) - 不屏蔽相关通道的终端计数中断。
  * @param: IE,错误中断掩码；可设置的值包括：IE_Mask(0x0) - 屏蔽相关通道的错误中断；
  *												IE_NotMask(0x01) - 不屏蔽相关通道的错误中断。
  *	
  * @retVal: void
  */
void DMA_ChannelConfig(DMA_TypeDef *DMAx, uint8_t Channel, int Halt, int Lock, int ITC, int IE)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_Channel(Channel));
	

	if(Channel== Channel_0)
	{
		DMAx->DMACC0Configuration &= ~((0x01U<<18)|(0x01U<<16)|(0x01U<<15)|(0x01U<<14));
		DMAx->DMACC0Configuration |= ((uint32_t)Halt<<18)|((uint32_t)Lock<<16)|((uint32_t)ITC<<15)|((uint32_t)IE<<14);
	}
	else
	{
		DMAx->DMACC1Configuration &= ~((0x01U<<18)|(0x01U<<16)|(0x01U<<15)|(0x01U<<14));
		DMAx->DMACC1Configuration |= ((uint32_t)Halt<<18)|((uint32_t)Lock<<16)|((uint32_t)ITC<<15)|((uint32_t)IE<<14);
	}
}

/**
  *
  * @brief: DMA控制传输类型设置。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: Flow_Control,控制传输类型，可设置的值包括：
  *						MtoM_DMA(0x0) - Memory-to-memory(Controller:DMA); MtoP_DMA(0x01) - Memory-to-peripheral(Controller:DMA);
  *						PtoM_DMA(0x02) - Peripheral-to-memory(Controller:DMA); SPtoDP_DMA(0x03) - Source peripheral-to-destination peripheral(Controller:DMA);
  *						SPtoDP_DP(0x04) - Source peripheral-to-destination peripheral(Controller:Destination peripheral);
  *						MtoP_P(0x05) - Memory-to-peripheral(Controller:Peripheral); PtoM_P(0x06) - Peripheral-to-memory(Controller:Peripheral);
  *						SPtoDP_SP(0x07) - Source peripheral-to-destination peripheral(Controller:Source peripheral)。
  *	
  * @retVal: void
  */
void DMA_ChannelConfig_Flow(DMA_TypeDef *DMAx,uint8_t Channel,int Flow_Control)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_Channel(Channel));

	if(Channel== Channel_0)
	{
		DMAx->DMACC0Configuration &= ~(0x07U<<11);
		DMAx->DMACC0Configuration |= ((uint32_t)Flow_Control<<11);
	}
	else
	{
		DMAx->DMACC1Configuration &= ~(0x07U<<11);
		DMAx->DMACC1Configuration |= ((uint32_t)Flow_Control<<11);
	}
}

/**
  *
  * @brief: 设置DMA目的地址外设和源地址外设并使能DMA通道。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: DestPeri,目的地址外设；可设置的值包括：0x4-SPI0-TX; 0x5 -SPI0-RX; 0x6 -SPI1-TX; 0x7 -SPI1-RX。
  * @param: SrcPeri,源地址外设；可设置的值包括：0x4-SPI0-TX; 0x5 -SPI0-RX; 0x6 -SPI1-TX; 0x7 -SPI1-RX。
  * @param: EN,是否使能DMA通道；可设置的值包括：DMA_Dis(0x0) - 不使能；DMA_En(0x01) - 使能。
  *	
  * @retVal: void
  */
void DMA_ChannelConfig_Peri(DMA_TypeDef *DMAx, uint8_t Channel, int DestPeri, int SrcPeri, int EN)
{	
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_Channel(Channel));
	
	if(Channel==Channel_0 )
	{
		DMAx->DMACC0Configuration &= ~((0x0fU<<6)|(0x0fU<<1)|(0x01U<<0));
		DMAx->DMACC0Configuration |= ((uint32_t)DestPeri<<6)|((uint32_t)SrcPeri<<1)|((uint32_t)EN<<0);
	}
	else
	{
		DMAx->DMACC1Configuration &= ~((0x0fU<<6)|(0x0fU<<1)|(0x01U<<0));
		DMAx->DMACC1Configuration |= ((uint32_t)DestPeri<<6)|((uint32_t)SrcPeri<<1)|((uint32_t)EN<<0);
	}
}


/**
  *
  * @brief: 设置链表传输的地址。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Channel,使用哪个通道；可设置的值包括：Channel_0(0x00) - 通道0；Channel_1 (0x01) - 通道1。
  * @param: LLL_Address,链表地址。
  * 
  * @retVal: void
  */
void DMA_LLI_List_Item(DMA_TypeDef* DMAx,uint8_t Channel, uint32_t LLL_Address)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	if(Channel== Channel_0)	
	{
		DMAx->DMACC0LLI |= (LLL_Address<<2);
	}
	else
	{
		DMAx->DMACC1LLI |= (LLL_Address<<2);
	}
}

/**
  *
  * @brief: DMA软件突发请求。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Breq,突发源。
  * 
  * @retVal: void
  */
void DMA_SoftwareBurstRequest(DMA_TypeDef* DMAx, uint16_t Breq)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	DMAx->DMACSoftBReq = Breq;
}

/**
  *
  * @brief: DMA软件单次请求。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: Sreq,软件单次请求源。
  * 
  * @retVal: void
  */
void DMA_SoftwareSingleRequest(DMA_TypeDef* DMAx, uint16_t Sreq)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	DMAx->DMACSoftSReq = Sreq;
}

/**
  *
  * @brief: DMA软件最后一次突发请求。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: LBreq,突发源。
  * 
  * @retVal: void
  */
void DMA_SoftwareLastBurstRequest(DMA_TypeDef* DMAx, uint16_t LBreq)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	DMAx->DMACSoftLBReq = LBreq;
}

/**
  *
  * @brief: DMA软件最后一次单次请求。
  *
  * @param: DMAx,STAR只有1组DMA,只能设置为STAR_DMA。
  * @param: LSreq,软件单次请求源。
  * 
  * @retVal: void
  */
void DMA_SoftwareLastSingleRequest(DMA_TypeDef* DMAx, uint16_t LSreq)
{
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	
	DMAx->DMACSoftLSReq = LSreq;
	
}


