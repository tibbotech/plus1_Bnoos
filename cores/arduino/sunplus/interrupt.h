#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "sp7021_hal_exti.h"
#include "sp7021_hal.h"


#ifdef __cplusplus
	#include <functional>

	typedef std::function<void(void)> callback_function_t;
	void sunplus_interrupt_enable(uint16_t pin, callback_function_t callback, uint32_t mode);

#endif

void sunplus_interrupt_enable(uint16_t pin, void (*callback)(void), uint32_t mode);
void sunplus_interrupt_disable(uint16_t pin);

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI5_IRQHandler(void);
void EXTI6_IRQHandler(void);
void EXTI7_IRQHandler(void);

#endif

