#ifndef __SYSTEM_SP70XX_H
#define __SYSTEM_SP70XX_H

#ifdef __cplusplus
 extern "C" {
#endif


extern uint32_t SystemCoreClock;          /*!< System Core1 Clock Frequency  */

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif 
