#include "Arduino.h"
#include "HardwareTimer.h"



#define MAX_RELOAD ((1 << 16) - 1) // Currently even 32b timers are used as 16b to have generic behavior
static timerObj_t *HardwareTimer_Handle[TIMER_NUM] = {NULL};
static callback_function_t callbacks[TIMER_NUM];


#if defined(TIM0_BASE)
static void TIM0_IRQHandler(void);
#endif

#if defined(TIM1_BASE)
static void TIM1_IRQHandler(void);
#endif

#if defined(TIM2_BASE)
static void TIM2_IRQHandler(void);
#endif

#if defined(TIM3_BASE)
static void TIM3_IRQHandler(void);
#endif

#if defined(TIM4_BASE)
static void TIM4_IRQHandler(void);
#endif

#if defined(TIM5_BASE)
static void TIM5_IRQHandler(void);
#endif

#if defined(TIM6_BASE)
static void TIM6_IRQHandler(void);
#endif

#if defined(TIM7_BASE)
static void TIM7_IRQHandler(void);
#endif

static timer_index_t get_timer_index(TIM_TypeDef *instance);



HardwareTimer::HardwareTimer(TIM_TypeDef *instance)
{
	uint32_t index = get_timer_index(instance);
	printf("TIM0 address= 0x%08p\n", instance);

	if (index == UNKNOWN_TIMER) 
	{
		Error_Handler();
	}

	HardwareTimer_Handle[index] = &_timerObj;
	_timerObj.handle.Instance = instance;
	_timerObj.__this = (void *)this;
	switch(index)
	{	
	#if defined(TIM0_BASE)
		case TIMER0_INDEX:
			_timerObj.handle.IrqHandle = &TIM0_IRQHandler;
			break;
	#endif	

	#if defined(TIM1_BASE)
		case TIMER1_INDEX:
			_timerObj.handle.IrqHandle = &TIM1_IRQHandler;
			break;
	#endif

	#if defined(TIM2_BASE)
		case TIMER2_INDEX:
			_timerObj.handle.IrqHandle = &TIM2_IRQHandler;
			break;
	#endif

	#if defined(TIM3_BASE)
		case TIMER3_INDEX:
			_timerObj.handle.IrqHandle = &TIM3_IRQHandler;
			break;
	#endif

	#if defined(TIM4_BASE)
		case TIMER4_INDEX:
			_timerObj.handle.IrqHandle = &TIM4_IRQHandler;
			break;
	#endif

	#if defined(TIM5_BASE)
		case TIMER5_INDEX:
			_timerObj.handle.IrqHandle = &TIM5_IRQHandler;
			break;
	#endif

	#if defined(TIM6_BASE)
		case TIMER6_INDEX:
			_timerObj.handle.IrqHandle = &TIM6_IRQHandler;
			break;
	#endif

	#if defined(TIM7_BASE)
		case TIMER7_INDEX:
			_timerObj.handle.IrqHandle = &TIM7_IRQHandler;
			break;
	#endif

	}
	_timerObj.handle.Lock = HAL_UNLOCKED;
	_timerObj.handle.State = HAL_TIM_STATE_RESET;
	_timerObj.handle.Init.Prescaler = 0;
	_timerObj.handle.Init.Counter = MAX_RELOAD;
	_timerObj.handle.Init.ReloadCounter = MAX_RELOAD;
	_timerObj.handle.Init.ClockSource = CLK_SYS_SRC;
	_timerObj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	_timerObj.pfcallback = NULL;
	HAL_TIM_Init(&(_timerObj.handle));
	
}


HardwareTimer::~HardwareTimer(void)
{
	uint32_t index = get_timer_index(_timerObj.handle.Instance);
	HAL_TIM_Stop(&(_timerObj.handle));
	HardwareTimer_Handle[index] = NULL;
	_timerObj.__this = NULL;
	_timerObj.pfcallback = NULL;

}


void HardwareTimer::pause(void)
{
	HAL_TIM_Stop(&(_timerObj.handle));
}

void HardwareTimer::resume(void)
{
	HAL_TIM_Start(&(_timerObj.handle));
}

void HardwareTimer::setPrescaleFactor(uint32_t prescaler)
{
	if (prescaler > 0)
		prescaler -= 1; 
	LL_TIM_SetPrescaler(_timerObj.handle.Instance, prescaler);
	
}


uint32_t HardwareTimer::getPrescaleFactor(void)
{
	return (LL_TIM_GetPrescaler(_timerObj.handle.Instance)+1);
}

uint32_t HardwareTimer::getTimerClkFreq(void)
{
	uint32_t u32Src = 0;
	uint32_t u32Feq = HSI_VALUE; 
	//u32Src = HAL_TIM_GetCLKSrc(&(_timerObj.handle));
	u32Src = LL_TIM_GetClockSource(_timerObj.handle.Instance);
	printf("u32Src = %d\n", u32Src);
	switch (u32Src)
	{
		case CLK_SYS_SRC:
			u32Feq = HSI_VALUE;
			break;
		case CLK_STC_SRC:
			break;
		case CLK_RTC_SRC:
			break;
		case CLK_EXT_SRC:
			u32Feq = HSE_VALUE/2; /*need to STC moule stc_config value(8-bit)*/
			break;
		case CLK_SLAVE_TRIG_SRC:
		case CLK_SLAVE_WRAP_SRC:
			u32Feq = HAL_TIM_GetMasterCLKFreq(&(_timerObj.handle)); 			/*need to acording the frontend timer src to caculate*/
			break;
		default:
			break;
	}

	return u32Feq;
}


void HardwareTimer::setCount(uint32_t val, TimerFormat_t format)
{
	uint32_t Prescalerfactor = 0;
	uint32_t u32Count_val = 0;
	
	Prescalerfactor = LL_TIM_GetPrescaler(_timerObj.handle.Instance)+1;

	switch(format){
		case MICROSEC_FORMAT:
			//u32Count_val = ((val * (getTimerClkFreq() / 1000000)) / Prescalerfactor);
			u32Count_val = ((((getTimerClkFreq() / 1000) / Prescalerfactor)-1)*val / 1000);
			break;

		case HERTZ_FORMAT:
			//u32Count_val = getTimerClkFreq() /(Prescalerfactor*val);
			u32Count_val = ((((getTimerClkFreq() / 1000) / Prescalerfactor)-1)*1000 / val);
			break;

		case TICK_FORMAT:
		default:
			u32Count_val  = val;
			break;
	}

	LL_TIM_SetCounter(_timerObj.handle.Instance, u32Count_val);
	LL_TIM_SetAutoReload(_timerObj.handle.Instance, u32Count_val);
}

uint32_t HardwareTimer::getCount(TimerFormat_t format)
{
	uint32_t count = LL_TIM_GetCounter(_timerObj.handle.Instance);
	uint32_t Prescalerfactor = LL_TIM_GetPrescaler(_timerObj.handle.Instance)+1;
	uint32_t reload_value = LL_TIM_GetAutoReload(_timerObj.handle.Instance);
	uint32_t return_value;

	switch(format){
		case MICROSEC_FORMAT:
			//return_value = (uint32_t)((count * Prescalerfactor * 1000000) / getTimerClkFreq());
			return_value = (count * 1000 / (((getTimerClkFreq() / 1000) / Prescalerfactor)-1));
			break;

		case HERTZ_FORMAT:
			//return_value = (uint32_t)(getTimerClkFreq() / (count  * Prescalerfactor));
			return_value = ((((getTimerClkFreq() / 1000) / Prescalerfactor)-1)*1000 / count);
			break;
		case TICK_FORMAT:
		default:
			return_value = reload_value - count;
			break;
	}

	return return_value;

}

void HardwareTimer::setOverflow(uint32_t val, TimerFormat_t format ) 
{
	uint32_t Prescalerfactor = 0;
	uint32_t u32Count_val = 0;
	
	Prescalerfactor = LL_TIM_GetPrescaler(_timerObj.handle.Instance)+1;

	switch(format){
		case MICROSEC_FORMAT:
			u32Count_val = ((val * (getTimerClkFreq() / 1000000)) / Prescalerfactor);
			break;

		case HERTZ_FORMAT:
			u32Count_val = getTimerClkFreq() /(Prescalerfactor*val);
			break;

		case TICK_FORMAT:
		default:
			u32Count_val  = val;
			break;
	}

	LL_TIM_SetAutoReload(_timerObj.handle.Instance, u32Count_val);

}

uint32_t HardwareTimer:: getOverflow(TimerFormat_t format)
{
	uint32_t count = LL_TIM_GetAutoReload(_timerObj.handle.Instance);
	uint32_t Prescalerfactor = LL_TIM_GetPrescaler(_timerObj.handle.Instance)+1;;
	uint32_t return_value;

	switch(format){
		case MICROSEC_FORMAT:
			return_value = (uint32_t)((count * Prescalerfactor * 1000000) / getTimerClkFreq());
			break;

		case HERTZ_FORMAT:
			return_value = (uint32_t)(getTimerClkFreq() / (count  * Prescalerfactor));
			break;
		case TICK_FORMAT:
		default:
			return_value = count;
			break;
	}

	return return_value;


}

void HardwareTimer::setClockSource(CLK_SRC src)
{
	LL_TIM_SetClockSource(_timerObj.handle.Instance, src);
}

uint32_t HardwareTimer::getClockSource(void)
{
	return LL_TIM_GetClockSource(_timerObj.handle.Instance);
}




#if 0
void HardwareTimer::attachInterrupt(callback_function_t callback)

{
		


	uint32_t index = get_timer_index(_timerObj.handle.Instance);
	if (index == UNKNOWN_TIMER)
		return;
	
	if (callbacks[index] != NULL)
		HAL_TIM_Stop(&(_timerObj.handle));

	callbacks[index] = callback;	


	
	if (_timerObj.pfcallback == NULL)
		_timerObj.pfcallback =callback;
	else{
		HAL_TIM_Stop(&(_timerObj.handle));
		_timerObj.pfcallback =callback;
	}

}
#endif	


void HardwareTimer::attachInterrupt(void (*callback)(void))
{
	uint32_t index = get_timer_index(_timerObj.handle.Instance);
	if (index == UNKNOWN_TIMER)
		return;
	//if (_timerObj.pfcallback != NULL)
	//	HAL_TIM_Stop(&(_timerObj.handle));
	_timerObj.pfcallback =callback;
	
}

void HardwareTimer:: detachInterrupt()
{
	uint32_t index = get_timer_index(_timerObj.handle.Instance);
	if (index == UNKNOWN_TIMER)
		return;
	
	HAL_TIM_Stop(&(_timerObj.handle));
	_timerObj.pfcallback = NULL;
	

} 



//extern "C" {

#if defined(TIM0_BASE)
static void TIM0_IRQHandler(void)
{
	if ((HardwareTimer_Handle[TIMER0_INDEX] != NULL)
		&& (HardwareTimer_Handle[TIMER0_INDEX]->pfcallback))
	{

		 HardwareTimer_Handle[TIMER0_INDEX]->pfcallback();
	}
}
#endif

#if defined(TIM1_BASE)
static void TIM1_IRQHandler(void)
{
	if ((HardwareTimer_Handle[TIMER1_INDEX] != NULL)
		&& (HardwareTimer_Handle[TIMER1_INDEX]->pfcallback))
	{

		 HardwareTimer_Handle[TIMER1_INDEX]->pfcallback();
	}

}
#endif

#if defined(TIM2_BASE)
static void TIM2_IRQHandler(void)
{
	if ((HardwareTimer_Handle[TIMER2_INDEX] != NULL)
		&& (HardwareTimer_Handle[TIMER2_INDEX]->pfcallback))
	{

		 HardwareTimer_Handle[TIMER2_INDEX]->pfcallback();
	}
}
#endif

#if defined(TIM3_BASE)
static void TIM3_IRQHandler(void)
{
	if ((HardwareTimer_Handle[TIMER3_INDEX] != NULL)
		&& (HardwareTimer_Handle[TIMER3_INDEX]->pfcallback))
	{

		 HardwareTimer_Handle[TIMER3_INDEX]->pfcallback();
	}
}
#endif

#if defined(TIM4_BASE)
static void TIM4_IRQHandler(void)
{
	if ((HardwareTimer_Handle[TIMER4_INDEX] != NULL)
		&& (HardwareTimer_Handle[TIMER4_INDEX]->pfcallback))
	{

		 HardwareTimer_Handle[TIMER4_INDEX]->pfcallback();
	}
}
#endif

#if defined(TIM5_BASE)
static void TIM5_IRQHandler(void)
{
	if ((HardwareTimer_Handle[TIMER5_INDEX] != NULL)
		&& (HardwareTimer_Handle[TIMER5_INDEX]->pfcallback))
	{

		 HardwareTimer_Handle[TIMER5_INDEX]->pfcallback();
	}
}
#endif


#if defined(TIM6_BASE)
static void TIM6_IRQHandler(void)
{
	if ((HardwareTimer_Handle[TIMER6_INDEX] != NULL)
		&& (HardwareTimer_Handle[TIMER6_INDEX]->pfcallback))
	{

		 HardwareTimer_Handle[TIMER6_INDEX]->pfcallback();
	}
}
#endif

#if defined(TIM7_BASE)
static void TIM7_IRQHandler(void)
{
	if ((HardwareTimer_Handle[TIMER7_INDEX] != NULL)
		&& (HardwareTimer_Handle[TIMER7_INDEX]->pfcallback))
	{

		 HardwareTimer_Handle[TIMER7_INDEX]->pfcallback();
	}
}
#endif

static timer_index_t get_timer_index(TIM_TypeDef *instance)
{
  timer_index_t index = UNKNOWN_TIMER;

#if defined(TIM0_BASE)
	if (instance == TIM0) {
	  index = TIMER0_INDEX;
	}
#endif

#if defined(TIM1_BASE)
  if (instance == TIM1) {
    index = TIMER1_INDEX;
  }
#endif
#if defined(TIM2_BASE)
  if (instance == TIM2) {
    index = TIMER2_INDEX;
  }
#endif
#if defined(TIM3_BASE)
  if (instance == TIM3) {
    index = TIMER3_INDEX;
  }
#endif
#if defined(TIM4_BASE)
  if (instance == TIM4) {
    index = TIMER4_INDEX;
  }
#endif
#if defined(TIM5_BASE)
  if (instance == TIM5) {
    index = TIMER5_INDEX;
  }
#endif
#if defined(TIM6_BASE)
  if (instance == TIM6) {
    index = TIMER6_INDEX;
  }
#endif
#if defined(TIM7_BASE)
  if (instance == TIM7) {
    index = TIMER7_INDEX;
  }
#endif

  return index;
}

//}

