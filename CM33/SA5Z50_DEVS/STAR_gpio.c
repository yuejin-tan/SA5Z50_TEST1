#include "STAR_gpio.h"
#include "STAR.h"

/**
  *
  * @brief: 将GPIO相关寄存器初始化为默认值。
  *
  * @param: GPIOx，使用哪组GPIO;由于STAR只提供1组GPIO，因此该参数只能设为STAR_GPIO0。
  *
  * @retVal: void
  */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    if (GPIOx == STAR_GPIO0)
    {
        GPIOx->OUTENCLR = 0xffffffff;
        GPIOx->ALTFUNCCLR = 0xffffffff;
        GPIOx->INTENCLR = 0xffffffff;
        GPIOx->INTTYPECLR = 0xffffffff;
        GPIOx->INTPOLCLR = 0xffffffff;
        GPIOx->DATAOUT = 0x0;
        GPIOx->DATA = 0x0;
    }
}

/**
  *
  * @brief: 将指定GPIO口设置为输入模式且读取输入电平。
  *
  * @param: GPIOx，使用哪组GPIO;由于STAR只提供1组GPIO，因此该参数只能设为STAR_GPIO0。
  * @param: GPIO_Pin_x，使用哪个GPIO;取值范围：GPIO_Pin_0~GPIO_Pin_31。
  *
  * @retVal: bitStatus，读取到的GPIO电平值;取值范围: 低电平-Bit_RESET(0)或高电平-Bit_SET(1)。
  * @notes: 一个IO被设置为输出后就不能再设置为输入了,设置为输入的可以再设置为输出。
  */
uint8_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
    uint8_t bitStatus = 0x00;

    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin_x));

    //GPIOx->OUTENCLR |= (1 << GPIO_Pin_x); //只要OUTENCLR被赋值，会将OUTENCLR和OUTENSET一起清零

    if ((GPIOx->DATA & (Bit_SET << GPIO_Pin_x)) == Bit_RESET)
    {
        bitStatus = (uint8_t)Bit_RESET;
    }
    else
    {
        bitStatus = (uint8_t)Bit_SET;
    }

    return bitStatus;
}

/**
  *
  * @brief: 设置指定GPIO口高电平输出。
  *
  * @param: GPIOx，使用哪组GPIO;由于STAR只提供1组GPIO，因此该参数只能设为STAR_GPIO0。
  * @param: GPIO_Pin_x，使用哪个GPIO;取值范围：GPIO_Pin_0~GPIO_Pin_31。
  *
  * @retVal: void
  */
void GPIO_SetBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{


    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin_x));


    GPIOx->OUTENSET |= (1 << GPIO_Pin_x);
    //GPIOx->ALTFUNCCLR  |= (1 << GPIO_Pin_x);	//只要ALTFUNCCLR被赋值，会将ALTFUNCCLR和ALTFUNCSET一起清零
    //GPIOx->INTENCLR |= (1 << GPIO_Pin_x);		//只要INTENCLR被赋值，会将INTENCLR和INTENSET一起清零

#if 1
    GPIOx->DATAOUT |= (1 << GPIO_Pin_x);
#else
    uint32_t gpioDatamask = 0x00;
    if (GPIOx == STAR_GPIO0)
    {
        gpioDatamask = (1 << GPIO_Pin_x % 8);

        if (GPIO_Pin_x <= 7)
        {
            GPIOx->MASKBYTE0[gpioDatamask] |= (1 << GPIO_Pin_x);
        }
        else if ((8 <= GPIO_Pin_x) && (GPIO_Pin_x <= 15))
        {
            GPIOx->MASKBYTE1[gpioDatamask] |= (1 << GPIO_Pin_x);
        }
        else if ((16 <= GPIO_Pin_x) && (GPIO_Pin_x <= 23))
        {
            GPIOx->MASKBYTE2[gpioDatamask] |= (1 << GPIO_Pin_x);
        }
        else if ((24 <= GPIO_Pin_x) && (GPIO_Pin_x <= 31))
        {
            GPIOx->MASKBYTE3[gpioDatamask] |= (1 << GPIO_Pin_x);
        }
    }
#endif
}

/**
  *
  * @brief: 设置指定GPIO口低电平输出。
  *
  * @param: GPIOx，使用哪组GPIO;由于STAR只提供1组GPIO，因此该参数只能设为STAR_GPIO0。
  * @param: GPIO_Pin_x，使用哪个GPIO;取值范围：GPIO_Pin_0~GPIO_Pin_31。
  *
  * @retVal: void
  */
void GPIO_ResetBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{


    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin_x));


    GPIOx->OUTENSET |= (1 << GPIO_Pin_x);
    //GPIOx->ALTFUNCCLR   |= (1 << GPIO_Pin_x); //只要ALTFUNCCLR被赋值，会将ALTFUNCCLR和ALTFUNCSET一起清零
    //GPIOx->INTENCLR     |= (1 << GPIO_Pin_x); //只要INTENCLR被赋值，会将INTENCLR和INTENSET一起清零
#if 1
    GPIOx->DATAOUT &= ~(1 << GPIO_Pin_x);
#else
    uint32_t gpioDatamask = 0x00;
    if (GPIOx == STAR_GPIO0)
    {
        gpioDatamask = (1 << GPIO_Pin_x % 8);

        if (GPIO_Pin_x <= 7)
        {
            GPIOx->MASKBYTE0[gpioDatamask] &= (~(1 << GPIO_Pin_x));
        }
        else if ((8 <= GPIO_Pin_x) && (GPIO_Pin_x <= 15))
        {
            GPIOx->MASKBYTE1[gpioDatamask] &= (~(1 << GPIO_Pin_x));
        }
        else if ((16 <= GPIO_Pin_x) && (GPIO_Pin_x <= 23))
        {
            GPIOx->MASKBYTE2[gpioDatamask] &= (~(1 << GPIO_Pin_x));
        }
        else if ((24 <= GPIO_Pin_x) && (GPIO_Pin_x <= 31))
        {
            GPIOx->MASKBYTE3[gpioDatamask] &= (~(1 << GPIO_Pin_x));
        }
    }
#endif
}

/**
  *
  * @brief: 设置指定GPIO口输出高电平或低电平。
  *
  * @param: GPIOx，使用哪组GPIO;由于STAR只提供1组GPIO，因此该参数只能设为STAR_GPIO0。
  * @param: GPIO_Pin_x，使用哪个GPIO;取值范围：GPIO_Pin_0~GPIO_Pin_31。
  * @param: BitVal，高电平(Bit_SET)或低电平(Bit_RESET);取值范围：Bit_SET(1)或Bit_RESET(0)。
  *
  * @retVal: void
  */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x, uint16_t BitVal)
{

    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin_x));
    assert_param(IS_GPIO_BIT_ACTION(BitVal));


    GPIOx->OUTENSET |= (1 << GPIO_Pin_x);
    //GPIOx->ALTFUNCCLR   |= (1 << GPIO_Pin_x); //只要ALTFUNCCLR被赋值，会将ALTFUNCCLR和ALTFUNCSET一起清零
    //GPIOx->INTENCLR     |= (1 << GPIO_Pin_x); //只要INTENCLR被赋值，会将INTENCLR和INTENSET一起清零

    if (BitVal != Bit_RESET)
    {
        GPIOx->DATAOUT |= (Bit_SET << GPIO_Pin_x);
    }
    else
    {
        GPIOx->DATAOUT &= (~(Bit_SET << GPIO_Pin_x));
    }
}

/**
  *
  * @brief: 设置指定GPIO口输出电平取反。
  *
  * @param: GPIOx，使用哪组GPIO;由于STAR只提供1组GPIO，因此该参数只能设为STAR_GPIO0。
  * @param: GPIO_Pin_x，使用哪个GPIO;取值范围：GPIO_Pin_0~GPIO_Pin_31。
  *
  * @retVal: void
  */
void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{


    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin_x));


    GPIOx->OUTENSET |= (1 << GPIO_Pin_x);
    //GPIOx->ALTFUNCCLR  |= (1 << GPIO_Pin_x );
    //GPIOx->INTENCLR  |= (1 << GPIO_Pin_x );

#if 1
    GPIOx->DATA ^= (Bit_SET << GPIO_Pin_x);
#else
    uint32_t gpioDatamask = 0x00;
    if (GPIOx == STAR_GPIO0)
    {
        gpioDatamask = (1 << GPIO_Pin_x % 8);

        if (GPIO_Pin_x <= 7)
        {
            GPIOx->MASKBYTE0[gpioDatamask] ^= (1 << GPIO_Pin_x);
        }
        else if ((8 <= GPIO_Pin_x) && (GPIO_Pin_x <= 15))
        {
            GPIOx->MASKBYTE1[gpioDatamask] ^= (1 << GPIO_Pin_x);
        }
        else if ((16 <= GPIO_Pin_x) && (GPIO_Pin_x <= 23))
        {
            GPIOx->MASKBYTE2[gpioDatamask] ^= (1 << GPIO_Pin_x);
        }
        else if ((24 <= GPIO_Pin_x) && (GPIO_Pin_x <= 31))
        {
            GPIOx->MASKBYTE3[gpioDatamask] ^= (1 << GPIO_Pin_x);
        }
    }
#endif
}


/**
  *
  * @brief: 设置GPIO的复用功能，如UART、ADC、Timer、I2C、SPI等。
  *
  * @param: GPIOx，使用哪组GPIO;由于STAR只提供1组GPIO，因此该参数只能设为STAR_GPIO0。
  * @param: GPIO_Remap，选择对哪个GPIO口进行复用，取值参考GPIO_Remap_define。
  * @param: NewState，是否使能复用，ENABLE：使能复用，DISABLE:不使能复用；取值范围：ENABLE或DISABLE。
  *
  * @retVal: void
  * @notes: 一个IO复用为某个功能后就不能再取消该复用功能。
  */
void GPIO_PinRemapConfig(GPIO_TypeDef* GPIOx, uint32_t  GPIO_Remap, FunctionalState NewState)
{

    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_REMAP(GPIO_Remap));
    assert_param(IS_FUNCTIONAL_STATE(NewState));


    if (NewState != DISABLE)
    {
        GPIOx->ALTFUNCSET |= GPIO_Remap;
    }
    else
    {
        //GPIOx->ALTFUNCCLR  |= GPIO_Remap; //注意!!!只要ALTFUNCCLR被赋值，会将ALTFUNCCLR整个清零
    }
}


/**
  *
  * @brief: 设置指定GPIO的中断触发方式。
  *
  * @param: GPIOx，使用哪组GPIO;由于STAR只提供1组GPIO，因此该参数只能设为STAR_GPIO0。
  * @param: GPIO_Pin_x，使用哪个GPIO;取值范围：GPIO_Pin_0~GPIO_Pin_31。
  * @param: INTTYPE，中断触发方式;
  *					取值范围：Bit_LOWLEVEL - 低电平触发；Bit_HIGHLEVEL - 高电平触发；
  *							  Bit_FALLINGEDGE - 下降沿触发；Bit_RISINGEDGE - 上升沿触发。
  * @param: NewState，是否使能中断，Interrupt_ENABLE：使能，Interrupt_DISABLE:不使能；
  *						取值范围：Interrupt_ENABLE或Interrupt_DISABLE。
  * @notes: 一个IO指定未某个中断触发方式后就不能再取消该设置。
  * @retVal: void
  */
void GPIO_PinInterruptTypeConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x,
    BitInterruptType INTTYPE, uint16_t NewState)
{

    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin_x));
    assert_param(IS_GPIO_BIT_INTTYPE(INTTYPE));
    assert_param(IS_INTERRUPT_STATE(NewState));


    if (NewState != Interrupt_DISABLE)
    {
        GPIOx->INTENSET |= (Interrupt_ENABLE << GPIO_Pin_x);
        switch (INTTYPE)
        {
        case  Bit_LOWLEVEL:
            GPIOx->INTTYPECLR |= (Interrupt_ENABLE << GPIO_Pin_x);
            GPIOx->INTPOLCLR |= (Interrupt_ENABLE << GPIO_Pin_x);
            break;
        case  Bit_HIGHLEVEL:
            GPIOx->INTTYPECLR |= (Interrupt_ENABLE << GPIO_Pin_x);
            GPIOx->INTPOLSET |= (Interrupt_ENABLE << GPIO_Pin_x);
            break;
        case  Bit_FALLINGEDGE:
            GPIOx->INTTYPESET |= (Interrupt_ENABLE << GPIO_Pin_x);
            GPIOx->INTPOLCLR |= (Interrupt_ENABLE << GPIO_Pin_x);
            break;
        case  Bit_RISINGEDGE:
            GPIOx->INTTYPESET |= (Interrupt_ENABLE << GPIO_Pin_x);
            GPIOx->INTPOLSET |= (Interrupt_ENABLE << GPIO_Pin_x);
            break;
        }
    }
    else
    {
        //GPIOx->INTENCLR |= (Interrupt_ENABLE << GPIO_Pin_x);
    }
}

/**
  *
  * @brief: 清除指定GPIO的中断请求标志。
  *
  * @param: GPIOx，使用哪组GPIO;由于STAR只提供1组GPIO，因此该参数只能设为STAR_GPIO0。
  * @param: GPIO_Pin_x，使用哪个GPIO;取值范围：GPIO_Pin_0~GPIO_Pin_31。
  *
  * @retVal: void
  */
void GPIO_PinInterruptClearFlag(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{

    assert_param(IS_GET_GPIO_PIN(GPIO_Pin_x));

    GPIOx->INTCLEAR |= (1 << GPIO_Pin_x); //注意!!!只要INTCLEAR被赋值，会将INTCLEAR整个清零
}

/**
  *
  * @brief: 设置指定GPIO口输出关闭。
  *
  * @param: GPIOx，使用哪组GPIO;由于STAR只提供1组GPIO，因此该参数只能设为STAR_GPIO0。
  * @param: GPIO_Pin_x，指定哪个GPIO口;取值范围：GPIO_Pin_0~GPIO_Pin_31。
  *
  * @retVal: void
  */
void GPIO_OutModeDisable(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin_x));

    GPIOx->OUTENCLR = (1 << GPIO_Pin_x);
}

/**
  *
  * @brief: 设置指定GPIO口输出开启。
  *
  * @param: GPIOx，使用哪组GPIO;由于STAR只提供1组GPIO，因此该参数只能设为STAR_GPIO0。
  * @param: GPIO_Pin_x，指定哪个GPIO口;取值范围：GPIO_Pin_0~GPIO_Pin_31。
  *
  * @retVal: void
  */
void GPIO_OutModeEnable(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin_x));

    GPIOx->OUTENSET = (1 << GPIO_Pin_x);
}
