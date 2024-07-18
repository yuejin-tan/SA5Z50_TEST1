#include "STAR_gpio.h"
#include "STAR.h"
#include "STAR_watchdog.h"


/**
  *
  * @brief: 配置WatchDog的加载值。
  *
  * @param: WATCHDOGx,STAR只有1个看门狗,只能设置为STAR_WATCHDOG。
  * @param: wdogload,要设置的加载值。
  * 
  * @retVal: void
  */
void WatchDog_Set_LOAD(WATCHDOG_TypeDef *WATCHDOGx, uint32_t wdogload)
{
	assert_param(IS_WatchDog_ALL_PERIPH(WATCHDOGx));
	
	WATCHDOGx->WDOGLOAD = wdogload;
}

/**
  *
  * @brief: 锁定WatchDog，禁止对WatchDog寄存器的写访问。
  *
  * @param: WATCHDOGx,STAR只有1个看门狗,只能设置为STAR_WATCHDOG。
  * 
  * @retVal: void
  */
void WatchDog_LOCK(WATCHDOG_TypeDef *WATCHDOGx)
{
	assert_param(IS_WatchDog_ALL_PERIPH(WATCHDOGx));
	
	WATCHDOGx->WDOGLOCK = 0;
}

/**
  *
  * @brief: 解锁WatchDog，允许对WatchDog寄存器的写访问。
  *
  * @param: WATCHDOGx,STAR只有1个看门狗,只能设置为STAR_WATCHDOG。
  * 
  * @retVal: void
  */
void WatchDog_UNLOCK(WATCHDOG_TypeDef *WATCHDOGx)
{
	assert_param(IS_WatchDog_ALL_PERIPH(WATCHDOGx));
	
	WATCHDOGx->WDOGLOCK = 0x1ACCE551;
}

/**
  *
  * @brief: WatchDog控制寄存器设置。
  *
  * @param: WATCHDOGx,STAR只有1个看门狗,只能设置为STAR_WATCHDOG。
  * @param: resen,是否使能WatchDog复位输出WDOGRES；使能时，不喂狗就会进行系统复位；
  *					可设置的值包括：RESEN_Disable(0x0) - 不使能；RESEN_Enable(0x01) - 使能。
  * @param: inten, 是否使能WatchDog计数器和中断WDOGINT；需使能WatchDog计数器WatchDog才会工作。
  *					可设置的值包括：INTEN_Disable(0x0) -不使能；INTEN_Enable(0x01) -使能。
  * 
  * @retVal: void
  */
void WatchDog_Set_CTRL(WATCHDOG_TypeDef* WATCHDOGx, int resen, int inten)
{
	assert_param(IS_WatchDog_ALL_PERIPH(WATCHDOGx));
	assert_param(IS_RESEN(resen)); 
	assert_param(IS_INTEN(inten));
	
	WATCHDOGx->WDOGCONTROL = ((uint32_t)resen<<1) | ((uint32_t)inten<<0);
}	

/**
  *
  * @brief: 清除WatchDog中断。
  *
  * @param: WATCHDOGx,STAR只有1个看门狗,只能设置为STAR_WATCHDOG。
  * 
  * @retVal: void
  */
void WatchDog_Set_INTCLR(WATCHDOG_TypeDef *WATCHDOGx)
{
	assert_param(IS_WatchDog_ALL_PERIPH(WATCHDOGx));
	
	WatchDog_UNLOCK(WATCHDOGx);
	WATCHDOGx->WDOGINTCLR = STAR_Watchdog_INTCLR_Msk;
	WatchDog_LOCK(WATCHDOGx);
}

/**
  *
  * @brief: 读取WatchDog计数器当前值。
  *
  * @param: WATCHDOGx,STAR只有1个看门狗,只能设置为STAR_WATCHDOG。
  * 
  * @retVal: wdogvalue,读取到的WatchDog计数器当前值。
  */
uint32_t WatchDog_Get_VALUE(WATCHDOG_TypeDef *WATCHDOGx)
{
	
	assert_param(IS_WatchDog_ALL_PERIPH(WATCHDOGx));
	
	return WATCHDOGx->WDOGVALUE; 
	
}

/**
  *
  * @brief: 读取WatchDog原始中断状态寄存器。
  *
  * @param: WATCHDOGx,STAR只有1个看门狗,只能设置为STAR_WATCHDOG。
  * 
  * @retVal: temp,读取到的寄存器值。
  */
uint32_t WatchDog_Get_RAWINTSTAT(WATCHDOG_TypeDef* WATCHDOGx)
{
	assert_param(IS_WatchDog_ALL_PERIPH(WATCHDOGx));
	
	return WATCHDOGx->WDOGRIS;
	
}

/**
  *
  * @brief: 读取WatchDog中断状态寄存器。
  *
  * @param: WATCHDOGx,STAR只有1个看门狗,只能设置为STAR_WATCHDOG。
  * 
  * @retVal: temp, 读取到的寄存器值。
  */      
uint32_t WatchDog_Get_MASKINTSTAT(WATCHDOG_TypeDef* WATCHDOGx)
{
	assert_param(IS_WatchDog_ALL_PERIPH(WATCHDOGx));
	
	return  WATCHDOGx->WDOGMIS;
	
}

