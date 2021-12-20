#ifndef __SP7021_HAL_STC_H__
#define __SP7021_HAL_STC_H__

#include "sp7021_arm926.h"
#include "sp7021_hal_def.h"
#include "sp70xx.h"
#include "sp7021_hal.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct
{
	STC_TypeDef			 *Instance;     /*!< Register base address             */
	uint32_t			ClockSource;
	uint32_t 			Prescaler;
	uint32_t			ExtDiv;
} STC_HandleTypeDef;
	
HAL_StatusTypeDef HAL_STC_Init(STC_HandleTypeDef *Hstc);
HAL_StatusTypeDef HAL_STC_SetPrescaler(STC_HandleTypeDef *Hstc, uint32_t u32Prescaler);
HAL_StatusTypeDef HAL_STC_SetExtDiv(STC_HandleTypeDef *Hstc, uint32_t u32div);
uint64_t HAL_STC_GetCounter(STC_HandleTypeDef *Hstc);
uint32_t HAL_STC_GetPrescaler(STC_HandleTypeDef *Hstc);
uint32_t HAL_STC_GetExtDiv(STC_HandleTypeDef *Hstc);



#ifdef __cplusplus
}
#endif

#endif

