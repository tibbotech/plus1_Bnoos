#ifndef __SP7021_LL_STC_H__
#define __SP7021_LL_STC_H__

#ifdef __cplusplus
extern "C" {
#endif


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
#define LL_STC_WriteReg(__INSTANCE__, __REG__, __VALUE__) WRITE_REG((__INSTANCE__)->__REG__, (__VALUE__))

/**
  * @brief  Read a value in TIM register.
  * @param  __INSTANCE__ TIM Instance
  * @param  __REG__ Register to be read
  * @retval Register value
  */
#define LL_STC_ReadReg(__INSTANCE__, __REG__) READ_REG((__INSTANCE__)->__REG__)

__STATIC_INLINE void LL_STC_SetClkSrc(STC_TypeDef *STCx, uint32_t source)
{
	MODIFY_REG(STCx->stc_prescale_val, STC_TRIG_SRC, source<<STC_TRIG_SRC_Pos);
}

__STATIC_INLINE void LL_STC_SetPrescaler(STC_TypeDef *STCx, uint32_t Prescaler)
{
	MODIFY_REG(STCx->stc_prescale_val, STC_PRESCALER, Prescaler<<STC_PRESCALER_Pos);
}

__STATIC_INLINE void LL_STC_SetExtDiv(STC_TypeDef *STCx, uint32_t Divdor)
{
	MODIFY_REG(STCx->stc_config, STC_EXT_DIV, Divdor<<STC_EXT_DIV_Pos);
}

__STATIC_INLINE uint64_t LL_STC_GetCounter(STC_TypeDef *STCx)
{
		
			return	(((uint64_t)STCx->stc_63_48<<48)| \
					 ((uint64_t)STCx->stc_47_32<<32)| \
					 ((uint64_t)STCx->stc_31_16<<16)| \
					 ((uint64_t)STCx->stc_15_0))
}

__STATIC_INLINE uint64_t LL_STC_GetPrescaler(STC_TypeDef *STCx)
{
	return (uint32_t)(READ_BIT(STCx->stc_prescale_val, STC_PRESCALER)>>STC_PRESCALER_Pos);
}

__STATIC_INLINE uint64_t LL_STC_GetExtDiv(STC_TypeDef *STCx)
{
	return (uint32_t)(READ_BIT(STCx->stc_config, STC_EXT_DIV)>>STC_EXT_DIV_Pos);
}


#ifdef __cplusplus
}
#endif

#endif

