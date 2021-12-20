#ifndef __SP7021_HAL_EXTI_H
#define __SP7021_HAL_EXTI_H

#ifdef __cplusplus
	extern "C" {
#endif

/* Include ------------------------------------------------------------------*/
#include "sp7021_hal_irq_ctrl.h"

#define IRQ_MODE_TRIG_EDGE_NONE  	0x00
#define IRQ_MODE_TRIG_EDGE_ACTIVE  	0x10

typedef struct
{
	IRQn_Type irqn;				//intr num: EXTI0_IRQn~EXTI7_IRQn
	uint32_t trigger;			//triggle type (default high-level)
} EXTI_HandleTypeDef;

HAL_StatusTypeDef HAL_EXTI_SetMode(EXTI_HandleTypeDef *hexti);
int HAL_EXTI_EdgePatch(EXTI_HandleTypeDef *hexti);
#ifdef __cplusplus
}
#endif
#endif
