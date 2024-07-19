#include "delay.h"
#include "stdint.h"

#include "STAR.h"

void delay_ms(uint32_t delay_ms)
{
    for (uint32_t i = 0;i < delay_ms;i++)
    {
        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
        {
            // wait
        }
    }
}
