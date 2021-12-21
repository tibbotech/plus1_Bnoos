#include "Arduino.h"
/*
	note to develop:
	only use timer2 and timer3 of every STC
	e.x. TIM0 is timer2 of STC
		 TIM1 is timer3 of STC
		 TIM2 is timer2 of STC_AV0
		 TIM3 is timer3 of STC_AV0 and so on....
*/
uint32_t start = 0;

void tim0_callback(void)
{
	
	printf("@@@@@@@timer0 callback @@@@@@@, tick= %lu\n", HAL_GetTick());
	start = HAL_GetTick();
	//printf("start:%lu\n", start);
}

void setup() 
{
	printf("%s, %s\n", __FILE__, __FUNCTION__);
	
//	Serial.begin(115200);
//  Serial.println("\timer interrupt\n ");
// 	Serial.print("%s\n\n\n");


  	uint32_t u32Prescal = 0xFFFF;
 	//HAL_Init();
 	HardwareTimer *timer0 = new HardwareTimer(TIM1);
	timer0->setPrescaleFactor(20200);
	//timer0->setPrescaleFactor(1000);
	u32Prescal = timer0->getPrescaleFactor();
	::printf("u32Prescal= 0x%x\n", u32Prescal);
	printf("freq = %d\n", timer0->getTimerClkFreq());
	timer0->setCount(10000-1, TICK_FORMAT);
	timer0->setOverflow(10000-1, TICK_FORMAT);
	//timer0->setCount(999, TICK_FORMAT);
	printf("counter = %d\n", timer0->getCount(TICK_FORMAT));
	timer0->attachInterrupt(tim0_callback);
	start = (uint32_t)HAL_GetTick();
	timer0->resume();
  
}



void loop() 
{
	//printf("%s, %s\n", __FILE__, __FUNCTION__);
}













