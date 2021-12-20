#ifndef __sp7021_LL_TIM_H
#define __sp7021_LL_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sp70xx.h"
#include "sp7021_arm926.h"
#include "sp7021_hal_def.h"
#include "irq_ctrl.h"



/** @defgroup TIM_LL_EM_WRITE_READ Common Write and read registers Macros
  * @{
  */
/**
  * @brief  Write a value in TIM register.
  * @param  __INSTANCE__ TIM Instance
  * @param  __REG__ Register to be written
  * @param  __VALUE__ Value to be written in the register
  * @retval None
  */
#define LL_TIM_WriteReg(__INSTANCE__, __REG__, __VALUE__) WRITE_REG((__INSTANCE__)->__REG__, (__VALUE__))

/**
  * @brief  Read a value in TIM register.
  * @param  __INSTANCE__ TIM Instance
  * @param  __REG__ Register to be read
  * @retval Register value
  */
#define LL_TIM_ReadReg(__INSTANCE__, __REG__) READ_REG((__INSTANCE__)->__REG__)




__STATIC_INLINE void LL_TIM_EnableCounter(TIM_TypeDef *TIMx)
{
	SET_BIT(TIMx->control, TIMER_GO);
}

__STATIC_INLINE void LL_TIM_DisableCounter(TIM_TypeDef *TIMx)
{
	CLEAR_BIT(TIMx->control, TIMER_GO);
}

__STATIC_INLINE uint32_t LL_TIM_IsEnabledCounter(TIM_TypeDef *TIMx)
{
  	return ((READ_BIT(TIMx->control, TIMER_GO) == (TIMER_GO)) ? 1UL : 0UL);
}


__STATIC_INLINE void LL_TIM_EnableARRPreload(TIM_TypeDef *TIMx)
{
  	SET_BIT(TIMx->control, TIMER_RPT);
}

__STATIC_INLINE void LL_TIM_DisableARRPreload(TIM_TypeDef *TIMx)
{
  	CLEAR_BIT(TIMx->control, TIMER_RPT);
}


__STATIC_INLINE uint32_t LL_TIM_IsEnabledARRPreload(TIM_TypeDef *TIMx)
{
	return ((READ_BIT(TIMx->control, TIMER_RPT) == (TIMER_RPT)) ? 1UL : 0UL);
}


__STATIC_INLINE void LL_TIM_SetClockSource(TIM_TypeDef *TIMx, uint32_t source)
{
  	MODIFY_REG(TIMx->control, TIMER_TRIG_SRC, source<<TIMER_TRIG_SRC_Pos);
}

__STATIC_INLINE uint32_t LL_TIM_GetClockSource(TIM_TypeDef *TIMx)
{
  	return (uint32_t)(READ_BIT(TIMx->control, TIMER_TRIG_SRC)>>TIMER_TRIG_SRC_Pos);
}

__STATIC_INLINE void LL_TIM_SetPrescaler(TIM_TypeDef *TIMx, uint32_t Prescaler)
{
   	WRITE_REG(TIMx->prescale_val, Prescaler);
}

__STATIC_INLINE uint32_t LL_TIM_GetPrescaler(TIM_TypeDef *TIMx)
{
  	return (uint32_t)(READ_REG(TIMx->prescale_val));
}

__STATIC_INLINE void LL_TIM_SetAutoReload(TIM_TypeDef *TIMx, uint32_t AutoReload)
{
  	WRITE_REG(TIMx->reload_val, AutoReload);
}

__STATIC_INLINE uint32_t LL_TIM_GetAutoReload(TIM_TypeDef *TIMx)
{
  	return (uint32_t)(READ_REG(TIMx->reload_val));
}

__STATIC_INLINE void LL_TIM_SetCounter(TIM_TypeDef *TIMx, uint32_t Counter)
{
  	WRITE_REG(TIMx->counter_val, Counter);
}


__STATIC_INLINE uint32_t LL_TIM_GetCounter(TIM_TypeDef *TIMx)
{
  	return (uint32_t)(READ_REG(TIMx->counter_val));
}


#ifdef __cplusplus
}
#endif

#endif




