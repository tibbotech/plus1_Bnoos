#include "Arduino.h"

/*
 * Test the high-level and edge-rising.
 * pin set:
 * 		output: GPIO simulates PWM waves	GPIO_P4_06(evb 38)
 * 		input : Externel interrupt0			GPIO_P4_07(evb 39)
 * Connect the PWM pin and Externel interrupt0 pin.
 * 		expected result:
 * 			HIGH: 	print every 3 seconds.
 * 			RISING: print every 6 seconds.
 *
 * 		if high-level trigger, pull down the level in irq_handler (simulate clearing the signal from  externel device)
 *
 * 		Notice: Edge trigger is simulated by software.
 */

GPIO_InitTypeDef gpio;
static int trigger;

void gpio_int_irq_callback()
{
/* 
	if external interrupt trigger mode is level,
	clear the interrupt signal of device when exit.
	here setting gpio low imitate clearing intr signal of the waveform.
 */
	if (trigger == HIGH)
	{
		::printf("exti high-level!\n");
		HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_LOW);
	}
	else if (trigger == RISING)
	{
		::printf("exti edge-rising!\n");
	}
}

void setup()
{
	trigger = RISING;
	
	gpio.Pin = GPIO_P4_06;
	gpio.opendrain = GPIO_OD_DISABLE;
	gpio.Mode = GPIO_OUTPUT_MODE;
	HAL_GPIO_Init(&gpio);
}

void loop()
{
	int i = 0;
	attachInterrupt(PIN_EXT_INT0, &gpio_int_irq_callback, trigger);
	if(trigger == RISING)
	{
		/* 5 pulses , 3 seconds per pulse*/
		i = 5;
		while(i)
		{
			HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_LOW);
			delay(3000);
			HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_HIGH);
			delay(3000);
			i--;
		}
		HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_LOW);
	}

	if(trigger == HIGH)
	{
		/* 1 pulses , 1 seconds high level*/
		i = 2;
		while(i)
		{
			HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_LOW);
			delay(1000);
			HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_HIGH);
			delay(1000);
			HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_LOW);
			i--;
		}
	}
	detachInterrupt(PIN_EXT_INT0);
}
