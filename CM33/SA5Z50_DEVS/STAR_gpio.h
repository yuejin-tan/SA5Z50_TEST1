#ifndef __STAR_GPIO_H
#define __STAR_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

  /*---------Include---------*/
#include "STAR.h"
#include "stdint.h"

/**
  * @defgroup GPIO_Exported_Types
  * @{
  */

#define IS_GPIO_ALL_PERIPH(PERIPH)  (((PERIPH) == STAR_GPIO0))



  /**
    * @brief  Bit_SET and Bit_RESET enumeration
    */

#define Bit_RESET 0U
#define Bit_SET 1U

#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))


    /**
      * @brief External interrupt trigger type
      */

  typedef enum
  {
    Bit_LOWLEVEL = 0,
    Bit_HIGHLEVEL,
    Bit_FALLINGEDGE,
    Bit_RISINGEDGE
  }BitInterruptType;
#define IS_GPIO_BIT_INTTYPE(INTTYPE) (((INTTYPE) == Bit_LOWLEVEL) || ((INTTYPE) == Bit_HIGHLEVEL) || \
																		((INTTYPE) == Bit_FALLINGEDGE)|| ((INTTYPE) == Bit_RISINGEDGE))


#define Interrupt_DISABLE 0U
#define Interrupt_ENABLE 1U

#define IS_INTERRUPT_STATE(STATE) (((STATE) == Interrupt_DISABLE) || ((STATE) == Interrupt_ENABLE))


  /**
    * @defgroup GPIO_pins_define
    * @{
    */
#define GPIO_Pin_0                  (0)  /*!< Pin 0 selected */
#define GPIO_Pin_1                  (1)  /*!< Pin 1 selected */
#define GPIO_Pin_2                  (2)  /*!< Pin 2 selected */
#define GPIO_Pin_3                  (3)  /*!< Pin 3 selected */
#define GPIO_Pin_4                  (4)  /*!< Pin 4 selected */
#define GPIO_Pin_5                  (5)  /*!< Pin 5 selected */
#define GPIO_Pin_6                  (6)  /*!< Pin 6 selected */
#define GPIO_Pin_7                  (7)  /*!< Pin 7 selected */
#define GPIO_Pin_8                  (8)  /*!< Pin 8 selected */
#define GPIO_Pin_9                  (9)  /*!< Pin 9 selected */
#define GPIO_Pin_10                (10)  /*!< Pin 10 selected */
#define GPIO_Pin_11                (11)  /*!< Pin 11 selected */
#define GPIO_Pin_12                (12)  /*!< Pin 12 selected */
#define GPIO_Pin_13                (13)  /*!< Pin 13 selected */
#define GPIO_Pin_14                (14)  /*!< Pin 14 selected */
#define GPIO_Pin_15                (15)  /*!< Pin 15 selected */

#define GPIO_Pin_16                (16)  /*!< Pin 16 selected */
#define GPIO_Pin_17                (17)  /*!< Pin 17 selected */
#define GPIO_Pin_18                (18)  /*!< Pin 18 selected */
#define GPIO_Pin_19                (19)  /*!< Pin 19 selected */
#define GPIO_Pin_20                (20)  /*!< Pin 20 selected */
#define GPIO_Pin_21                (21)  /*!< Pin 21 selected */
#define GPIO_Pin_22                (22)  /*!< Pin 22 selected */
#define GPIO_Pin_23                (23)  /*!< Pin 23 selected */
#define GPIO_Pin_24                (24)  /*!< Pin 24 selected */
#define GPIO_Pin_25                (25)  /*!< Pin 25 selected */
#define GPIO_Pin_26                (26)  /*!< Pin 26 selected */
#define GPIO_Pin_27                (27)  /*!< Pin 27 selected */
#define GPIO_Pin_28                (28)  /*!< Pin 28 selected */
#define GPIO_Pin_29                (29)  /*!< Pin 29 selected */
#define GPIO_Pin_30                (30)  /*!< Pin 30 selected */
#define GPIO_Pin_31                (31)  /*!< Pin 31 selected */
#define GPIO_Pin_All               ((uint32_t)0xFFFFFFFF)  /*!< All pins selected */


#define IS_GPIO_PIN(PIN) ((((PIN) & (uint16_t)0x00) == 0x00) && ((PIN) != (uint16_t)0x00))

#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || ((PIN) == GPIO_Pin_2) || ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || ((PIN) == GPIO_Pin_6) || ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || ((PIN) == GPIO_Pin_9) || ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) ||((PIN) == GPIO_Pin_12) ||((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || ((PIN) == GPIO_Pin_15)||((PIN) == GPIO_Pin_16) || \
                              ((PIN) == GPIO_Pin_17) || ((PIN) == GPIO_Pin_18)||((PIN) == GPIO_Pin_19) || \
                              ((PIN) == GPIO_Pin_20) || ((PIN) == GPIO_Pin_21)||((PIN) == GPIO_Pin_22) || \
                              ((PIN) == GPIO_Pin_23) || ((PIN) == GPIO_Pin_24)||((PIN) == GPIO_Pin_25) || \
                              ((PIN) == GPIO_Pin_26) || ((PIN) == GPIO_Pin_27)||((PIN) == GPIO_Pin_28) || \
                              ((PIN) == GPIO_Pin_29) || ((PIN) == GPIO_Pin_30)||((PIN) == GPIO_Pin_31))
    /**
      * @}
      */

      /**
      * @defgroup GPIO_Remap_define
      * @{
      */
#define GPIO_Remap_TIM0EXTIN        ((uint32_t)0x00000001)
#define GPIO_Remap_TIM1EXTIN        ((uint32_t)0x00000002)
#define GPIO_Remap_USART0_RXD       ((uint32_t)0x00000004)
#define GPIO_Remap_USART0_TXD       ((uint32_t)0x00000008)

#define GPIO_Remap_USART1_RXD       ((uint32_t)0x00000010)
#define GPIO_Remap_USART1_TXD       ((uint32_t)0x00000020)
#define GPIO_Remap_ADCETR           ((uint32_t)0x00000040)
#define GPIO_Remap_SPI0_CLK_OUT     ((uint32_t)0x00000080)

#define GPIO_Remap_SPI0_SEL         ((uint32_t)0x00000100)
#define GPIO_Remap_SPI0_DATA0       ((uint32_t)0x00000200)
#define GPIO_Remap_SPI0_DATA1       ((uint32_t)0x00000400)
#define GPIO_Remap_SPI1_CLK_OUT     ((uint32_t)0x00000800)

#define GPIO_Remap_SPI1_SEL         ((uint32_t)0x00001000)
#define GPIO_Remap_SPI1_DATA0       ((uint32_t)0x00002000)
#define GPIO_Remap_SPI1_DATA1       ((uint32_t)0x00004000)
#define GPIO_Remap_I2C0SCL          ((uint32_t)0x00008000)

#define GPIO_Remap_I2C0SDA          ((uint32_t)0x00010000)
#define GPIO_Remap_SPI0_DATA2       ((uint32_t)0x00020000)
#define GPIO_Remap_SPI0_DATA3       ((uint32_t)0x00040000)
#define GPIO_Remap_SPI1_DATA2       ((uint32_t)0x00080000)

#define GPIO_Remap_SPI1_DATA3       ((uint32_t)0x00100000)
#define GPIO_Remap_ADC1_ETR         ((uint32_t)0x00200000)
#define GPIO_Remap_USART2_RXD 			((uint32_t)0x00400000)
#define GPIO_Remap_USART2_TXD				((uint32_t)0x00800000)


#define GPIO_Remap_SPI1_CLK_IN			((uint32_t)0x04000000)
#define GPIO_Remap_SPI1_SEL_IN			((uint32_t)0x08000000)

#define GPIO_Remap_SPI0_CLK_IN			((uint32_t)0x10000000)
#define GPIO_Remap_SPI0_SEL_IN			((uint32_t)0x20000000)

#define IS_GPIO_REMAP(REMAP) 		 (((REMAP) == GPIO_Remap_TIM0EXTIN  ) || ((REMAP) == GPIO_Remap_TIM1EXTIN )  ||\
                                  ((REMAP) == GPIO_Remap_USART0_RXD ) || ((REMAP) ==  GPIO_Remap_USART0_TXD )  ||\
                                  ((REMAP) == GPIO_Remap_USART1_RXD ) || ((REMAP) ==  GPIO_Remap_USART1_TXD )  ||\
                                  ((REMAP) ==  GPIO_Remap_ADCETR  ) || ((REMAP) ==  GPIO_Remap_SPI0_CLK_OUT  )  ||\
                                  ((REMAP) == GPIO_Remap_SPI0_SEL  ) || ((REMAP) ==  GPIO_Remap_SPI0_DATA0 )  ||\
                                  ((REMAP) == GPIO_Remap_SPI0_DATA1 ) || ((REMAP) ==  GPIO_Remap_SPI0_DATA2 )  ||\
                                  ((REMAP) == GPIO_Remap_SPI0_DATA3 ) || ((REMAP) ==  GPIO_Remap_SPI1_CLK_OUT )  ||\
                                  ((REMAP) == GPIO_Remap_SPI1_SEL  ) || ((REMAP) ==  GPIO_Remap_SPI1_DATA0 )  ||\
                                  ((REMAP) == GPIO_Remap_SPI1_DATA1 ) || ((REMAP) ==  GPIO_Remap_SPI1_DATA2 )  ||\
                                  ((REMAP) == GPIO_Remap_SPI1_DATA3 ) || ((REMAP) ==  GPIO_Remap_I2C0SCL ) ||\
                                  ((REMAP) == GPIO_Remap_I2C0SDA ))		||	((REMAP) ==  GPIO_Remap_ADC1_ETR ) ||\
																	 ((REMAP) == GPIO_Remap_USART2_RXD ) || ((REMAP) ==  GPIO_Remap_USART2_TXD ) ||\
                                  ((REMAP) == GPIO_Remap_SPI1_CLK_IN ))		||	((REMAP) ==  GPIO_Remap_SPI1_SEL_IN ) ||\
																	((REMAP) == GPIO_Remap_SPI0_CLK_IN ))		||	((REMAP) ==  GPIO_Remap_SPI0_SEL_IN )





  void GPIO_DeInit(GPIO_TypeDef* GPIOx);
  uint8_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);
  void GPIO_SetBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);
  void GPIO_ResetBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);
  void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x, uint16_t BitVal);
  void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);
  void GPIO_PinRemapConfig(GPIO_TypeDef* GPIOx, uint32_t  GPIO_Remap, FunctionalState NewState);
  void GPIO_PinInterruptTypeConfig(GPIO_TypeDef* CM3DS_MPS2_GPIOx, uint16_t GPIO_Pin_x, BitInterruptType INTTYPE, uint16_t NewState);
  void GPIO_PinInterruptClearFlag(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);

#ifdef __cplusplus
}
#endif

#endif /* __STAR_GPIO_H */
