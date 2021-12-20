#ifndef HARDWARETIMER_H_
#define HARDWARETIMER_H_


#include "sunplus_ll.h"
#include "sunplus_def.h"


typedef enum {
	TICK_FORMAT, // default
	MICROSEC_FORMAT,
	HERTZ_FORMAT,
} TimerFormat_t;


typedef enum {
#if defined(TIM0_BASE)
  TIMER0_INDEX,
#endif
#if defined(TIM1_BASE)
  TIMER1_INDEX,
#endif
#if defined(TIM2_BASE)
  TIMER2_INDEX,
#endif
#if defined(TIM3_BASE)
  TIMER3_INDEX,
#endif
#if defined(TIM4_BASE)
  TIMER4_INDEX,
#endif
#if defined(TIM5_BASE)
  TIMER5_INDEX,
#endif
#if defined(TIM6_BASE)
  TIMER6_INDEX,
#endif
#if defined(TIM7_BASE)
  TIMER7_INDEX,
#endif
  TIMER_NUM,
  UNKNOWN_TIMER = 0XFFFF
} timer_index_t;


#ifdef __cplusplus
#include <functional>
//using callback_function_t = std::function<void(void)>;
typedef std::function<void(void)> callback_function_t;





class HardwareTimer {
	public:
		HardwareTimer(TIM_TypeDef *instance);
		~HardwareTimer(void);
		void pause(void);  
		void resume(void);
		void setClockSource(CLK_SRC src=CLK_SYS_SRC);
		uint32_t getClockSource(void);
	 	void setPrescaleFactor(uint32_t prescaler);
		uint32_t getPrescaleFactor(void);
		void setCount(uint32_t val, TimerFormat_t format = TICK_FORMAT);
		uint32_t getCount(TimerFormat_t format = TICK_FORMAT); 
		void setPreloadEnable(bool value);
		void setOverflow(uint32_t val, TimerFormat_t format = TICK_FORMAT); // set AutoReload register depending on format provided
		uint32_t getOverflow(TimerFormat_t format = TICK_FORMAT); // return overflow depending on format provided
		//void attachInterrupt(callback_function_t callback); 
		void attachInterrupt(void (*callback)(void));
		void detachInterrupt(); 
		uint32_t getTimerClkFreq();   	

	private:
		timerObj_t _timerObj;
			
};
#endif

#endif

