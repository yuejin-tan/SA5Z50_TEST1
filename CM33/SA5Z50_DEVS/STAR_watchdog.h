#ifndef __STAR_WATCHDOG_H
#define __STAR_WATCHDOG_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#include "STAR.h"
	 
	  
#define IS_WatchDog_ALL_PERIPH(PERIPH)  ((PERIPH) == STAR_WATCHDOG))

	 
#define RESEN_Enable                			 1
#define RESEN_Disable               			 0
#define IS_RESEN(RESEN) 					(((RESEN) == RESEN_Enable ) || ((RESEN) == RESEN_Disable ))

#define INTEN_Enable            					 1
#define INTEN_Disable           					 0
#define IS_INTEN(INTEN) 				 (((INTEN) ==  RESEN_Enable ) ||((INTEN) ==  RESEN_Disable ))

	 
void WatchDog_Set_LOAD(WATCHDOG_TypeDef *WATCHDOGx,uint32_t wdogload);
uint32_t WatchDog_Get_VALUE(WATCHDOG_TypeDef *WATCHDOGx);
void WatchDog_Set_CTRL(WATCHDOG_TypeDef *WATCHDOGx,int resen,int inten);
void WatchDog_Set_INTCLR(WATCHDOG_TypeDef *WATCHDOGx); 
void WatchDog_UNLOCK(WATCHDOG_TypeDef *WATCHDOGx);
void WatchDog_LOCK(WATCHDOG_TypeDef *WATCHDOGx);

uint32_t WatchDog_Get_RAWINTSTAT(WATCHDOG_TypeDef *WATCHDOGx);
uint32_t WatchDog_Get_MASKINTSTAT(WATCHDOG_TypeDef *WATCHDOGx);	 


#ifdef __cplusplus
}
#endif

#endif

