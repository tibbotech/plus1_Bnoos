
#include "sp7021_hal_tim.h"


#ifdef __cplusplus
extern "C" {
#endif


static IRQn_Type HAL_TIM_GetIRQ(TIM_TypeDef *tim)
{
	IRQn_Type IRQn = MAX_IRQ_n;
	if ((tim == NULL) || (tim == (TIM_TypeDef*)0xFFFFFFFF))
		return IRQn;

	switch ((uint32_t)tim)
	{
		#ifdef TIM0_BASE
		case  (uint32_t)TIM0_BASE:
			IRQn = STC_TIMER2_IRQn;
			break;
		#endif	

		#ifdef TIM1_BASE
		case  (uint32_t)TIM1_BASE:
			IRQn = STC_TIMER3_IRQn;
			break;
		#endif	

		#ifdef TIM2_BASE
		case  (uint32_t)TIM2_BASE:
			IRQn = STC_AV0_TIMER2_IRQn;
			break;
		#endif

		#ifdef TIM3_BASE
			case  (uint32_t)TIM3_BASE:
				IRQn = STC_AV0_TIMER3_IRQn;
				break;
		#endif

		#ifdef TIM4_BASE
			case  (uint32_t)TIM4_BASE:
				IRQn = STC_AV1_TIMER2_IRQn;
				break;
		#endif

		#ifdef TIM5_BASE
			case  (uint32_t)TIM5_BASE:
				IRQn = STC_AV1_TIMER3_IRQn;
				break;
		#endif

		#ifdef TIM6_BASE
			case  (uint32_t)TIM6_BASE:
				IRQn = STC_AV2_TIMER2_IRQn;
				break;
		#endif

		#ifdef TIM7_BASE
			case  (uint32_t)TIM7_BASE:
				IRQn = STC_AV2_TIMER3_IRQn;
				break;
		#endif

		default:
        	//_Error_Handler("TIM: Unknown timer IRQn", (int)tim);
        break;
	}
	return IRQn;
}

timerObj_t *HAL_TIM_Get_timer_obj(TIM_HandleTypeDef *htim)
{
  timerObj_t *obj;
  obj = (timerObj_t *)((char *)htim - offsetof(timerObj_t, handle));
  return (obj);
}

void HAL_TIM_EnableTimerClock(TIM_HandleTypeDef *htim)
{

#ifdef TIM0_BASE || TIM1_BASE
	if((htim->Instance == TIM0) || (htim->Instance == TIM1))
	{
		HAL_Module_Clock_enable(STC0, 1);
		HAL_Module_Clock_gate(STC0, 1);
		HAL_Module_Reset(STC0, 1);
			
	}
#endif

#ifdef TIM2_BASE || TIM3_BASE
	if((htim->Instance == TIM2) || (htim->Instance == TIM3))
	{
		HAL_Module_Clock_enable(STC_AV0, 1);
		HAL_Module_Clock_gate(STC_AV0, 1);
		HAL_Module_Reset(STC_AV0, 1);
			
	}
#endif

#ifdef TIM4_BASE || TIM5_BASE
	if((htim->Instance == TIM4) || (htim->Instance == TIM5))
	{
		HAL_Module_Clock_enable(STC_AV1, 1);
		HAL_Module_Clock_gate(STC_AV1, 1);
		HAL_Module_Reset(STC_AV1, 1);
			
	}
#endif

#ifdef TIM6_BASE || TIM7_BASE
	if((htim->Instance == TIM6) || (htim->Instance == TIM7))
	{
		HAL_Module_Clock_enable(STC_AV2, 1);
		HAL_Module_Clock_gate(STC_AV2, 1);
		HAL_Module_Reset(STC_AV2, 1);
			
	}
#endif

}


void HAL_TIM_DisableTimerClock(TIM_HandleTypeDef *htim)
{

#ifdef TIM0_BASE || TIM1_BASE
	if((htim->Instance == TIM0) || (htim->Instance == TIM1))
	{
		HAL_Module_Clock_enable(STC0, 0);
		HAL_Module_Clock_gate(STC0, 0);
		HAL_Module_Reset(STC0, 0);
			
	}
#endif

#ifdef TIM2_BASE || TIM3_BASE
	if((htim->Instance == TIM2) || (htim->Instance == TIM3))
	{
		HAL_Module_Clock_enable(STC_AV0, 0);
		HAL_Module_Clock_gate(STC_AV0, 0);
		HAL_Module_Reset(STC_AV0, 0);
			
	}
#endif

#ifdef TIM4_BASE || TIM5_BASE
	if((htim->Instance == TIM4) || (htim->Instance == TIM5))
	{
		HAL_Module_Clock_enable(STC_AV1, 0);
		HAL_Module_Clock_gate(STC_AV1, 0);
		HAL_Module_Reset(STC_AV1, 0);
			
	}
#endif

#ifdef TIM6_BASE || TIM7_BASE
	if((htim->Instance == TIM6) || (htim->Instance == TIM7))
	{
		HAL_Module_Clock_enable(STC_AV2, 0);
		HAL_Module_Clock_gate(STC_AV2, 0);
		HAL_Module_Reset(STC_AV2, 0);
			
	}
#endif

}



HAL_StatusTypeDef HAL_TIM_Init(TIM_HandleTypeDef *htim)
{
	IRQn_Type irqn =  MAX_IRQ_n;
		
	if (htim == NULL)
  	{
    	return HAL_ERROR;
  	}

	assert_param(IS_TIM_INSTANCE(htim->Instance));

	if (htim->State == HAL_TIM_STATE_RESET)
	{
		htim->Lock = HAL_UNLOCKED;
		irqn = HAL_TIM_GetIRQ(htim->Instance);
		IRQ_SetHandler(irqn, htim->IrqHandle);
		IRQ_Enable(irqn);
	}
	htim->State = HAL_TIM_STATE_BUSY;
	htim->Instance->control = 0;
	htim->Instance->prescale_val = 0;
	htim->Instance->reload_val = 0;
	htim->Instance->counter_val = 0;
	TIM_SetConfig(htim->Instance, &htim->Init);
	
	/* Initialize the TIM state*/
	htim->State = HAL_TIM_STATE_READY;
	
	return HAL_OK;
}


HAL_StatusTypeDef HAL_TIM_DeInit(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* Disable the TIM Peripheral Clock */
  MODIFY_REG(htim->Instance->control, TIMER_GO, 0<<TIMER_GO_Pos);

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}


void TIM_SetConfig(TIM_TypeDef *TIMx, TIM_InitTypeDef *Structure)
{
	MODIFY_REG(TIMx->control, TIMER_TRIG_SRC, Structure->ClockSource<<TIMER_TRIG_SRC_Pos);
	MODIFY_REG(TIMx->control, TIMER_RPT, Structure->AutoReloadPreload<<TIMER_RPT_pos);
	TIMx->counter_val = Structure->Counter;
	TIMx->reload_val = Structure->ReloadCounter;
	
}


HAL_TIM_StateTypeDef HAL_TIM_GetState(TIM_HandleTypeDef *htim)
{
  	return htim->State;
}


HAL_StatusTypeDef HAL_TIM_Start(TIM_HandleTypeDef *htim)
{
	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(htim->Instance));

	/* Set the TIM state */
	htim->State = HAL_TIM_STATE_BUSY;

	MODIFY_REG(htim->Instance->control, TIMER_GO, 1<<TIMER_GO_Pos);

	/* Change the TIM state*/
	htim->State = HAL_TIM_STATE_READY;

	/* Return function status */
	return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_Stop(TIM_HandleTypeDef *htim)
{
	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(htim->Instance));

	/* Set the TIM state */
	htim->State = HAL_TIM_STATE_BUSY;
	

	MODIFY_REG(htim->Instance->control, TIMER_GO, 0<<TIMER_GO_Pos);

	/* Change the TIM state*/
	htim->State = HAL_TIM_STATE_READY;

	/* Return function status */
	return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_SetPrescaler(TIM_HandleTypeDef *htim, uint32_t u32Prescaler)
{
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	htim->Instance->prescale_val = u32Prescaler;
	htim->Init.Prescaler = u32Prescaler;
	return HAL_OK;
	
}

uint32_t HAL_TIM_GetPrescaler(TIM_HandleTypeDef *htim)
{
	assert_param(IS_TIM_INSTANCE(htim->Instance));
 	return htim->Instance->prescale_val;

}

HAL_StatusTypeDef HAL_TIM_setCount(TIM_HandleTypeDef *htim, uint32_t u32Count)
{
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	htim->Instance->counter_val = u32Count;
	htim->Instance->reload_val = u32Count;
	return HAL_OK;
}

uint32_t HAL_TIM_GetCount(TIM_HandleTypeDef *htim)
{
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	return htim->Instance->counter_val;
}




uint32_t HAL_TIM_GetCLKSrc(TIM_HandleTypeDef *htim)
{
	uint32_t u32Src = 0;
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	return (READ_BIT(htim->Instance->control, TIMER_TRIG_SRC)>>TIMER_TRIG_SRC_Pos);
	
}

uint32_t HAL_TIM_GetMasterCLKFreq(TIM_HandleTypeDef *htim)
{
	TIM_TypeDef *hMtim = 0;
	uint32_t u32Feq = 0;
	uint32_t u32Src = 0;
	uint32_t u32Prescaler = 0;
	uint32_t u32Counter = 0;
		
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	switch((uint32_t)htim->Instance){
#ifdef TIM0_BASE || TIM1_BASE	
		case TIM0_BASE:
			hMtim = TIM1;
			break;
		case TIM1_BASE:
			hMtim = TIM0;
			break;
#endif
#ifdef TIM2_BASE || TIM3_BASE
		case TIM2_BASE:
			hMtim = TIM3;
			break;
		case TIM3_BASE:
			hMtim = TIM2;
			break;
#endif
#ifdef TIM4_BASE || TIM5_BASE
		case TIM4_BASE:
			hMtim = TIM5;
			break;
		case TIM5_BASE:
			hMtim = TIM4;
			break;
#endif

#ifdef TIM6_BASE || TIM7_BASE
		case TIM6_BASE:
			hMtim = TIM7;
			break;
		case TIM7_BASE:
			hMtim = TIM6;
			break;
#endif

		default:
			return u32Feq;
			break;
	}

	
	u32Src = READ_BIT(hMtim->control, TIMER_TRIG_SRC)>>TIMER_TRIG_SRC_Pos;
	u32Prescaler = hMtim->prescale_val;
	u32Counter = hMtim->counter_val;

	switch (u32Src){
		case CLK_SYS_SRC:
			u32Feq = HSI_VALUE;
			break;
		case CLK_STC_SRC:
			break;
		case CLK_RTC_SRC:
			break;
		case CLK_EXT_SRC:
				u32Feq = HSE_VALUE/2;
			break;
		default:
			u32Feq = HSI_VALUE;
			break;

	}
	
	u32Feq = u32Feq/(u32Prescaler+1);
	if ((READ_BIT(htim->Instance->control, TIMER_TRIG_SRC)>>TIMER_TRIG_SRC_Pos)
		==CLK_SLAVE_WRAP_SRC){
		if(u32Counter != 0)
			u32Feq /= u32Counter;
	}	
	return u32Feq;
}

HAL_StatusTypeDef HAL_TIM_Enable_Interrupt(TIM_HandleTypeDef *htim)
{
	IRQn_Type irqn =  MAX_IRQ_n;
		
	if (htim == NULL)
  	{
    	return HAL_ERROR;
  	}

	assert_param(IS_TIM_INSTANCE(htim->Instance));
	irqn = HAL_TIM_GetIRQ(htim->Instance);
	if (IRQ_Enable(irqn) == 0)
		return HAL_OK;
	else
		return HAL_ERROR;
}


HAL_StatusTypeDef HAL_TIM_Disable_Interrupt(TIM_HandleTypeDef *htim)
{
	IRQn_Type irqn =  MAX_IRQ_n;
		
	if (htim == NULL)
	{
		return HAL_ERROR;
	}

	assert_param(IS_TIM_INSTANCE(htim->Instance));
	irqn = HAL_TIM_GetIRQ(htim->Instance);
	if (IRQ_Disable(irqn) == 0)
		return HAL_OK;
	else
		return HAL_ERROR;

}


#ifdef __cplusplus
}
#endif

