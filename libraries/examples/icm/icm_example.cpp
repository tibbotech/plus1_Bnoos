#include "Arduino.h"
#include "icm.h"
#include "sp7021_hal_pwm.h"

void setup()
{
	/*config pwm*/
	PWM_InitTypeDef PWM_Init;
	PWM_Init.Pin = GPIO_P1_03;
	PWM_Init.pwm_num = PWM0;
	PWM_Init.pwm_freq = 8000;
	PWM_Init.duty_cycle = 50;
	HAL_PWM_INIT(&PWM_Init);
	HAL_PWM_ENABLE(PWM0);
	
	ICM_Initialization();
}

void loop()
{
}

