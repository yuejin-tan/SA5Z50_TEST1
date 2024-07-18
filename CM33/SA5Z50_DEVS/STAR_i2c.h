#ifndef __STAR_I2C_H
#define __STAR_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

/*----------include------------*/
#include "STAR.h"

/** 
  * @brief  I2C Init structure definition  
  */
typedef struct
{
  uint32_t I2C_ClockSpeed;          /*!< Specifies the clock frequency.
                                         This parameter must be set to a value lower than 400kHz */

  uint32_t I2C_Mode;                /*!< Specifies the I2C mode.
                                         This parameter can be a value of @ref I2C_mode */

  uint32_t I2C_DutyCycle;           /*!< Specifies the I2C fast mode duty cycle.
                                         This parameter can be a value of @ref I2C_duty_cycle_in_fast_mode */
										 	
  uint32_t I2C_OwnAddressMode;
										 
  uint32_t I2C_OwnAddress;         /*!< Specifies the first device own address.
                                         This parameter can be a 7-bit or 10-bit address. */

  uint32_t I2C_SlvAddressMode;

  uint32_t I2C_SlvAddress;
										 
  uint32_t I2C_Ack;                 /*!< Enables or disables the acknowledgement.
                                         This parameter can be a value of @ref I2C_acknowledgement */

  uint32_t I2C_AcknowledgedAddress; /*!< Specifies if 7-bit or 10-bit address is acknowledged.
                                         This parameter can be a value of @ref I2C_acknowledged_address */
	
} I2C_InitTypeDef;

typedef struct
{
	uint32_t I2C_EXTI_Type;
		
} I2C_EXTITypeDef;

/**
  * @}
  */ 
#define I2C_Speed_Mode_Pos 0
#define I2C_clock_divide_factor_Pos 3
#define I2C_SDA_Delay_count_selection_pos 16
#define I2C_SDA_delay_count_Pos 20
/**
  * @defgroup I2C_clockSpeed
  * @{
  */

//usual:   standard 100K;   fast 400k:    high  3.4M  
//most devices do not support high speeds 0x00e00190
#define I2C_Speed_Standard          			((uint32_t)0x00e00190)
#define I2C_Speed_Fast 							((uint32_t)0x00708069)
#define I2C_Speed_High 							((uint32_t)0x00220012)
#define IS_I2C_ClockSpeed_STATE(STATE) (((STATE) == I2C_Speed_Standard) || \
	                                                 ((STATE) == I2C_Speed_Fast) ||\
	                                                 ((STATE) == I2C_Speed_High))

/**
  * @}
  */ 

/**
  * @defgroup I2C_mode
  * @{
  */
#define I2C_Mode_Master            ((uint16_t)0x8000)
#define I2C_Mode_Slave             ((uint16_t)0x0000)
#define IS_I2C_Mode_STATE(STATE) (((STATE) == I2C_Mode_Master) ||\
	                                                     ((STATE) == I2C_Mode_Slave))

/**
  * @}
  */ 

/** @defgroup I2C_OwnAddress
  * @{
  */
  
#define I2C_OwnAddress_7bit       ((uint16_t)0x0000)
#define I2C_OwnAddress_10bit      ((uint16_t)0x0001)
#define IS_I2C_OwnAddress_STATE(STATE) (((STATE) == I2C_OwnAddress_7bit) || \
	                            ((STATE) == I2C_OwnAddress_10bit))
/**
  *@}
  */

/** @defgroup I2C_SlvAddress
  * @{
  */
  
#define I2C_SlvAddress_7bit       ((uint16_t)0x0000)
#define I2C_SlvAddress_10bit      ((uint16_t)0x0001)
#define IS_I2C_SlvAddress_STATE(STATE) (((STATE) == I2C_SlvAddress_7bit) || \
	                            ((STATE) == I2C_SlvAddress_10bit))
/**
  *@}
  */

#define OwnAddr_Reset   ((uint16_t)0xF801)

/** @defgroup I2C_acknowledgement
  * @{
  */

#define I2C_Ack_Enable                  ((uint16_t)0x1000)
#define I2C_Ack_Disable                 ((uint16_t)0x0000)
#define IS_I2C_ACK_STATE(STATE) (((STATE) == I2C_Ack_Enable) || \
                                 ((STATE) == I2C_Ack_Disable))
/**
  * @}
  */

/** @defgroup I2C_AcknowledgedAddress
  * @{
  */
  
#define I2C_AcknowledgedAddress_7bit       ((uint8_t)0x00)
#define I2C_AcknowledgedAddress_10bit      ((uint8_t)0x01)
#define IS_I2C_AcknowledgedAddress_STATE(STATE) (((STATE) == I2C_AcknowledgedAddress_7bit) || \
	                            ((STATE) == I2C_AcknowledgedAddress_10bit))
/**
  *@}
  */

/** @defgroup I2C_REG_STATUS1
  * @{
  */
#define start_bit_det              	((uint32_t)0x00000001)
#define addr_7bit_match      		((uint32_t)0x00000002)
#define addr_10bit_math       		((uint32_t)0x00000004)
#define tx_fifo_full               	((uint32_t)0x00000008)//((uint32_t)0x00000010)
#define tx_fifo_empty            	((uint32_t)0x00000010) //((uint32_t)0x00000020)
#define rx_fifo_full               	((uint32_t)0x00000200)
#define rx_fifo_empty              	((uint32_t)0x00000400)
#define stop_bit_det               	((uint32_t)0x00008000)
#define tc_flag                    	((uint32_t)0x00010000)
#define data_received              	((uint32_t)0x00020000)
#define IS_I2C_STATUS1(STATUS1) (((STATUS1) == start_bit_det) || ((STATUS1) == addr_7bit_match) || \
                                                           ((STATUS1) == addr_10bit_math) ||((STATUS1) ==  tx_fifo_full) || \
                                                           ((STATUS1) == tx_fifo_empty) ||((STATUS1) ==  stop_bit_det) || \
                                                           ((STATUS1) == tc_flag) ||((STATUS1) ==  data_received))	|| \
															((STATUS1) == rx_fifo_full) ||((STATUS1) ==  rx_fifo_empty))

/**
  * @}
  */

/** @defgroup I2C_REG_STATUS2
  * @{
  */
#define i2c_hgc            ((uint16_t)0x0001)
#define i2c_hs_mode   		 ((uint16_t)0x0002)
#define arbitri_lost       ((uint16_t)0x0004)
#define ack_failure      	 ((uint16_t)0x0008)
#define bus_busy         	 ((uint16_t)0x0010)
#define bus_error_int  		 ((uint16_t)0x0020)
#define tx_int             ((uint16_t)0x0040)
#define rx_int             ((uint16_t)0x0080)
#define tc_int             ((uint16_t)0x0100)
#define stop_det_int    	 ((uint16_t)0x0200)
#define IS_I2C_STATUS2(STATUS2) (((STATUS2) == i2c_hgc) || ((STATUS2) == i2c_hs_mode) ||\
                                                          ((STATUS2) == arbitri_lost) ||((STATUS2) == ack_failure) ||\
                                                          ((STATUS2) == bus_busy) ||((STATUS2) == bus_error_int ) ||\
                                                          ((STATUS2) ==  tx_int) ||((STATUS2) == rx_int) ||\
                                                          ((STATUS2) == tc_int) ||((STATUS2) == stop_det_int))
/**
  * @}
  */

/** @defgroup I2C_Exported_Constants
  * @{
  */
#define IS_I2C_ALL_PERIPH(PERIPH) ((PERIPH) == STAR_I2C)

/**
  * @}
  */

#define I2C_IT_TX				((uint8_t)0x02)
#define I2C_IT_ERR				((uint8_t)0x80)
#define I2C_IT_RX				((uint16_t)0x0200)
#define I2C_IT_TC     			((uint16_t)0x2000)
#define I2C_IT_STOP				((uint16_t)0x4000)
#define IS_I2C_CONFIG_IT(I2C_IT) (((I2C_IT) == I2C_IT_TX) ||\
	                                			((I2C_IT) == I2C_IT_ERR) || \
	                                			((I2C_IT) == I2C_IT_RX) || \
	                                			((I2C_IT) == I2C_IT_TC) || \
	                                			((I2C_IT) == I2C_IT_STOP))

/** @defgroup I2C_transfer_direction 
  * @{
  */

#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#define  I2C_Direction_Receiver         ((uint8_t)0x01)
#define IS_I2C_DIRECTION(DIRECTION) (((DIRECTION) == I2C_Direction_Transmitter) || \
                                     ((DIRECTION) == I2C_Direction_Receiver))
/**
  * @}
  */

/**
  * @defgroup I2C_Exported_Functions
  * @{
  */

void I2C_DeInit(I2C_TypeDef *I2Cx);
void I2C_Init(I2C_TypeDef *I2Cx, uint16_t Mode, uint32_t Speed, uint32_t OwnAddress);
void I2C_SlvaddrConfig(I2C_TypeDef *I2Cx, uint32_t SlvAddress);
void I2C_GenerateSTART(I2C_TypeDef *I2Cx, FunctionalState NewState );
void I2C_TxEnable( I2C_TypeDef *I2Cx, FunctionalState NewState);
void I2C_RxEnable(I2C_TypeDef *I2Cx,FunctionalState NewState);
void I2C_GenerateSTOP(I2C_TypeDef *I2Cx, FunctionalState NewState);
void I2C_ITConfig(I2C_TypeDef* I2Cx,uint16_t I2C_IT,FunctionalState NewState);
void I2C_TxIT_TxbufferLevelConfig(I2C_TypeDef* I2Cx, uint8_t TxbufferWlevel);
void I2C_RxIT_RxbufferLevelConfig(I2C_TypeDef* I2Cx,uint8_t RxbufferWlevel);
void I2C_TxRxDataNumConfig(I2C_TypeDef *I2Cx,uint16_t Num);
void I2C_SendData(I2C_TypeDef *I2Cx,uint8_t Byte);
void I2C_WaitSendDataFinish(I2C_TypeDef *I2Cx);
void I2C_WaitReceiveReady(I2C_TypeDef *I2Cx);
uint8_t I2C_ReceiveData(I2C_TypeDef *I2Cx);
uint32_t I2C_GetStatus1Values(I2C_TypeDef *I2Cx);
uint16_t I2C_GetStatus2Values(I2C_TypeDef *I2Cx);
ITStatus I2C_GetITStatus(I2C_TypeDef *I2Cx,uint32_t I2C_IT);
ITStatus I2C_CheckStatus(I2C_TypeDef *I2Cx,uint32_t Status1);
void I2C_WaitTxFIFOIsNotFull (I2C_TypeDef* I2Cx);
void I2C_WaitBusIdle (I2C_TypeDef* I2Cx);
FlagStatus I2C_CheckACKIsFail(I2C_TypeDef* I2Cx);
void I2C_SoftwareResetCmd(I2C_TypeDef* I2Cx,FunctionalState NewState);


/**
  * @}
  */

 #ifdef __cplusplus
}
#endif
  
#endif /* __STAR_I2C_H */

