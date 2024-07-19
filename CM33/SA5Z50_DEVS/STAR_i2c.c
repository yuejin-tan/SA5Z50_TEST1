#include "STAR_i2c.h"
#include "STAR_rcc.h"
#include "STAR_gpio.h"
#include "STAR.h"

/**
  *
  * @brief: 将I2C初始化为默认配置。
  *
  * @param: I2Cx,设置为STAR_I2C。
  *
  * @retVal: void
  */
void I2C_DeInit(I2C_TypeDef* I2Cx)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  if (I2Cx == STAR_I2C)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C, DISABLE);
  }
}

/**
  *
  * @brief: I2C初始化配置。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: Mode,I2C模式；I2C_Mode_Master - I2C主模式; I2C_Mode_Slave - I2C从模式;
  * @param: Speed,I2C工作时钟速率模式；
  *					可设置的值:
  *						I2C_Speed_Standard - 标准速率模式；
  *						I2C_Speed_Fast - 快速速率模式；
  *						I2C_Speed_High - 高速速率模式。
  *	@param: OwnAddress,主机地址。
  *
  * @retVal: void
  */
void I2C_Init(I2C_TypeDef* I2Cx, uint16_t Mode, uint32_t Speed, uint32_t OwnAddress)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_ClockSpeed_STATE(Speed));

  I2Cx->CONTROL = 0x80000000;
  I2Cx->CONTROL = 0x00000000;
  I2Cx->CONTROL |= Mode | (0UL << STAR_I2C_SWRST_Pos);

  //I2Cx->FREQDIV =0;
  I2Cx->FREQDIV |= Speed;
  //|0x000000|(00UL << I2C_Speed_Mode_Pos)
  //|(0x39 << I2C_clock_divide_factor_Pos) 
  //|(11UL << I2C_SDA_Delay_count_selection_pos)
  //|(0x0  << I2C_SDA_delay_count_Pos);
  //I2Cx->OWNADDR =0;
  I2Cx->OWNADDR |= OwnAddress;
}

/**
  *
  * @brief: I2C从机地址设置。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: SlvAddress,要设置的从机地址。读操作或写操作由SlvAddress[15]位确定，该位为0表示写，为1表示读。
  *
  * @retVal: void
  */
void I2C_SlvaddrConfig(I2C_TypeDef* I2Cx, uint32_t SlvAddress)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  I2Cx->SLVADDR = SlvAddress;
}

/**
  *
  * @brief: 生成I2C起始信号。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: NewState,是否使能生成I2C起始信号；ENABLE - 使能；DISABLE - 不使能。
  *
  * @retVal: void
  */
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    I2Cx->CONTROL |= (1 << STAR_I2C_START_SET_Pos);
  }
  else
  {
    I2Cx->CONTROL &= ~(1U << STAR_I2C_START_SET_Pos);
  }
}

/**
  *
  * @brief: 使能I2C发送。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: NewState,是否使能I2C发送；ENABLE - 使能；DISABLE - 不使能。
  *
  * @retVal: void
  */
void I2C_TxEnable(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    I2Cx->CONTROL |= (1 << STAR_I2C_TX_ENA_Pos);
  }
  else
  {
    I2Cx->CONTROL &= ~(1U << STAR_I2C_TX_ENA_Pos);
  }
}

/**
  *
  * @brief: 使能I2C接收。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: NewState,是否使能I2C接收；ENABLE - 使能；DISABLE - 不使能。
  *
  * @retVal: void
  */
void I2C_RxEnable(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    I2Cx->CONTROL |= (1 << STAR_I2C_RX_ENA_Pos);
  }
  else
  {
    I2Cx->CONTROL &= ~(1U << STAR_I2C_RX_ENA_Pos);
  }
}

/**
  *
  * @brief: 生成I2C停止信号。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: NewState,是否生成I2C停止信号；ENABLE - 使能；DISABLE - 不使能。
  *
  * @retVal: void
  */
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    I2Cx->CONTROL |= (1 << STAR_I2C_STOP_SET_Pos);
    //while ((I2Cx-> STATUS1 & (1UL << 15)) == 0);  //stop bit detect
  }
  else
  {
    I2Cx->CONTROL &= ~(1U << STAR_I2C_STOP_SET_Pos);
  }
}

/**
  *
  * @brief: I2C中断配置。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: I2C_IT,I2C中断触发类型；
  *					可设置的值包括：
  *						I2C_IT_ERR(0x80) - I2C总线错误中断；
  *						I2C_IT_TC(0x2000) - I2C发送完成中断；
  *						I2C_IT_STOP(0x4000) - I2C停止中断；
  *						I2C_IT_TX(0x02)-I2C发送中断；
  *						I2C_IT_RX(0x0200)- I2C接收中断。
  * @param: NewState,是否使能中断；ENABLE - 使能；DISABLE - 不使能。
  *
  * @retVal: void
  */
void I2C_ITConfig(I2C_TypeDef* I2Cx, uint16_t I2C_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_I2C_CONFIG_IT(I2C_IT));

  if (NewState != DISABLE)
  {
    I2Cx->CONTROL |= I2C_IT;
  }
  else
  {
    I2Cx->CONTROL &= ~I2C_IT;
  }
}

/**
  *
  * @brief: I2C发送中断tx buffer门限配置。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: TxbufferWlevel, 发送buffer中断触发门限ctrl_tx_buff_wlevel；当TX FIFO占用的空间大于该值时将触发I2C发送中断；
  *						可设置的值为0/1/2/3。
  *
  * @retVal: void
  */
void I2C_TxIT_TxbufferLevelConfig(I2C_TypeDef* I2Cx, uint8_t TxbufferWlevel)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  if (TxbufferWlevel < 4)
  {
    I2Cx->CONTROL &= ~(0x03U << STAR_I2C_TX_BUFFER_LEVEL_Pos);
    I2Cx->CONTROL |= ((uint32_t)TxbufferWlevel << STAR_I2C_TX_BUFFER_LEVEL_Pos);
  }


}

/**
  *
  * @brief: I2C接收中断rx buffer门限配置。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: RxbufferWlevel, 接收buffer中断触发门限ctrl_rx_buff_wlevel；当RX FIFO占用的空间大于该值时将触发I2C接收中断；
  *						可设置的值为0/1/2/3。
  *
  * @retVal: void
  */
void I2C_RxIT_RxbufferLevelConfig(I2C_TypeDef* I2Cx, uint8_t RxbufferWlevel)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  if (RxbufferWlevel < 4)
  {
    I2Cx->CONTROL &= ~(0x03U << STAR_I2C_RX_BUFFER_LEVEL_Pos);
    I2Cx->CONTROL |= ((uint32_t)RxbufferWlevel << STAR_I2C_RX_BUFFER_LEVEL_Pos);
  }

}

/**
  *
  * @brief: 设置I2C发送或接收数据的个数。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: Num,需要发送或接收数据的个数。
  *
  * @retVal: void
  */
void I2C_TxRxDataNumConfig(I2C_TypeDef* I2Cx, uint16_t Num)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  I2Cx->CONTROL &= ~(0x0fU << STAR_I2C_TX_NUM_Pos);
  I2Cx->CONTROL |= (uint32_t)(Num << STAR_I2C_TX_NUM_Pos);
}

/**
  *
  * @brief: 使用I2C发送数据。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: Byte,需要发送的数据。
  *
  * @retVal: void
  */
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Byte)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  I2Cx->TXDATA = Byte;
}

/**
  *
  * @brief: 等待I2C发送完成。
  *
  * @param: I2Cx,设置为STAR_I2C。
  *
  * @retVal: void
  */
void I2C_WaitSendDataFinish(I2C_TypeDef* I2Cx)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  while ((I2Cx->STATUS1 & (1 << 4)) == 0);	//wait until tx fifo is empty
  while ((I2Cx->STATUS1 & (1 << 16)) == 0);		   //transmit complete flag
}

/**
  *
  * @brief: I2C等待接收数据，直到RX FIFO非空。
  *
  * @param: I2Cx,设置为STAR_I2C。
  *
  * @retVal: void
  */
void I2C_WaitReceiveReady(I2C_TypeDef* I2Cx)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  while ((I2Cx->STATUS1 & 0x00000400));
}

/**
  *
  * @brief: I2C接收数据。
  *
  * @param: I2Cx,设置为STAR_I2C。
  *
  * @retVal: Rx_Byte,I2C接收到的数据。
  */
uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  return (uint8_t)I2Cx->RXDATA;

}

/**
  *
  * @brief: 获取I2C状态寄存器1的值。
  *
  * @param: I2Cx,设置为STAR_I2C。
  *
  * @retVal: Values,获取到的状态寄存器值。
  */
uint32_t I2C_GetStatus1Values(I2C_TypeDef* I2Cx)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  return I2Cx->STATUS1;

}

/**
  *
  * @brief: 获取I2C状态寄存器2的值。
  *
  * @param: I2Cx,设置为STAR_I2C。
  *
  * @retVal: values,获取到的状态寄存器值。
  */
uint16_t I2C_GetStatus2Values(I2C_TypeDef* I2Cx)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  return (uint16_t)I2Cx->STATUS2;

}

/**
  *
  * @brief: 获取I2C指定中断是否产生。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: I2C_IT,I2C中断触发类型；
  *					可设置的值包括：
  *						I2C_IT_ERR(0x80) - I2C总线错误中断；
  *						I2C_IT_TC(0x2000) - I2C发送完成中断；
  *						I2C_IT_STOP(0x4000) - I2C停止中断；
  *						I2C_IT_TX(0x02)中断不能使用该函数进行设置，需要使用I2C_IT_TxbufferConfig 设置；
  *						I2C_IT_RX(0x0200)中断不能使用该函数进行设置，需要使用I2C_IT_RxbufferConfig 设置。
  *
  *
  * @retVal: bitStatus,获取到的状态，SET - 指定状态已发生；RESET - 指定状态未发生。
  */
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx, uint32_t I2C_IT)
{
  ITStatus bitStatus = RESET;
  uint16_t itStatusOffset = 0;
  uint32_t enablestatus = 0;

  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_CONFIG_IT(I2C_IT));

  enablestatus = (uint32_t)(I2Cx->CONTROL & I2C_IT); //获取对应的中断使能状态

  switch (I2C_IT)
  {
  case I2C_IT_ERR:
    itStatusOffset = 5;
    break;
  case I2C_IT_TC:
    itStatusOffset = 8;
    break;
  case I2C_IT_STOP:
    itStatusOffset = 9;
    break;
  case I2C_IT_TX:
    itStatusOffset = 6;
    break;
  case I2C_IT_RX:
    itStatusOffset = 7;
    break;
  default:

    break;
  }
  if ((itStatusOffset != 0) && ((I2Cx->STATUS2 & (1 << itStatusOffset)) != RESET) && enablestatus)
  {
    bitStatus = SET;
  }
  else
  {
    bitStatus = RESET;
  }

  return bitStatus;
}

/**
  *
  * @brief: 检测I2C指定状态是否发生。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: Status1,要检测的状态类型；
  *					可设置的值包括:
  *						start_bit_det - 起始位信号被检测;
  *						addr_7bit_match - 地址7位发送/接收匹配;	addr_10bit_math - 地址10位发送/接收匹配；
  *						tx_fifo_full - 发送FIFO已满;	tx_fifo_empty - 发送FIFO已空；
  *						rx_fifo_full - 接收FIFO已满;	rx_fifo_empty - 接收FIFO已空；
  *						stop_bit_det - 停止位信号被检测; tc_flag - 发送完成；
  *						data_received - 数据到达。
  *
  *
  * @retVal: bitStatus,获取到的状态，SET - 指定状态已发生；RESET - 指定状态未发生。
  */
ITStatus I2C_CheckStatus(I2C_TypeDef* I2Cx, uint32_t Status1)
{
  ITStatus bitStatus = RESET;

  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_STATUS1(Status1));

  if ((I2Cx->STATUS1 & Status1) != RESET)
  {
    bitStatus = SET;
  }
  else
  {
    bitStatus = RESET;
  }
  return bitStatus;
}

/**
  *
  * @brief: 等待I2C总线为IDLE状态。
  *
  * @param: I2Cx,设置为STAR_I2C。
  *
  * @retVal: void
  */
void I2C_WaitBusIdle(I2C_TypeDef* I2Cx)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  while ((I2Cx->STATUS2 & (1 << 4)));
}

/**
  *
  * @brief: 等待直到I2C的Tx FIFO不为满状态。
  *
  * @param: I2Cx,设置为STAR_I2C。
  *
  * @retVal: void
  */
void I2C_WaitTxFIFOIsNotFull(I2C_TypeDef* I2Cx)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  while ((I2Cx->STATUS1 & (1 << 3)));
}

/**
  *
  * @brief: 检测ACK是否识别。
  *
  * @param: I2Cx,设置为STAR_I2C。
  *
  * @retVal: isFail,获取到的ACK识别状态，SET - ACK失败；RESET - ACK正常。
  */
FlagStatus I2C_CheckACKIsFail(I2C_TypeDef* I2Cx)
{
  FlagStatus isFail;

  if (I2Cx->STATUS2 & 0x08)
  {
    isFail = SET;
  }
  else
  {
    isFail = RESET;
  }

  return isFail;
}

/**
  *
  * @brief: I2C软件复位设置。
  *
  * @param: I2Cx,设置为STAR_I2C。
  * @param: NewState,是否使能复位；ENABLE - 使能复位；DISABLE - 释放复位。
  *
  * @retVal: void
  * others: 使能I2C软件复位后，需要及时释放复位；
  */
void I2C_SoftwareResetCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  if (NewState != DISABLE)
  {
    I2Cx->CONTROL |= (1U << STAR_I2C_SWRST_Pos);
  }
  else
  {
    I2Cx->CONTROL &= ~(1U << STAR_I2C_SWRST_Pos);
  }

}

