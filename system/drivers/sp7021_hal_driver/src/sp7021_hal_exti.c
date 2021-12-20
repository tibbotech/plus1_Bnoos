#include "sp7021_hal_exti.h"
#include "sp7021_hal_conf.h"
/* There is BUG in EXIT edge triggerger, workaround for the trouble */
HAL_StatusTypeDef HAL_EXTI_SetMode(EXTI_HandleTypeDef *hexti)
{
	int idx = 0;
	int mask = 0;
	
	if ((hexti->irqn < 120) || (hexti->irqn > 127))
	{
		return HAL_ERROR;
	}
	
	idx = hexti->irqn / 32;
	mask = (1 << (hexti->irqn % 32));
	
	if(hexti->trigger & IRQ_MODE_TRIG_EDGE_RISING)
	{
		SP_IRQ_CTRL->type[idx] &= ~mask;//edge->level
		SP_IRQ_CTRL->polarity[idx] &= ~mask;//high
	}
	else if(hexti->trigger & IRQ_MODE_TRIG_EDGE_FALLING)
	{
		SP_IRQ_CTRL->type[idx] &= ~mask;
		SP_IRQ_CTRL->polarity[idx] |= mask;
	}
	else
	{
		IRQ_SetMode(hexti->irqn ,hexti->trigger);
	}
	return HAL_OK;
}

int HAL_EXTI_EdgePatch(EXTI_HandleTypeDef *hexti)
{
	int idx = 0;
	int mask = 0;
	
	idx = hexti->irqn / 32;
	mask = (1 << (hexti->irqn % 32));
	
	if(hexti->trigger & IRQ_MODE_TRIG_EDGE_ACTIVE)//second times entry irq
	{
		if(hexti->trigger & IRQ_MODE_TRIG_EDGE_RISING)
		{
			SP_IRQ_CTRL->polarity[idx] &= ~mask;//low->high
		}
		else
		{
			SP_IRQ_CTRL->polarity[idx] |= mask;//high->low
		}
		hexti->trigger &= ~IRQ_MODE_TRIG_EDGE_ACTIVE;
		return 0;
	}
	/* first time entry irq */
	else if(hexti->trigger == IRQ_MODE_TRIG_EDGE_RISING)
	{
		SP_IRQ_CTRL->polarity[idx] |= mask;//high->low
		hexti->trigger |= IRQ_MODE_TRIG_EDGE_ACTIVE;
	}
	else if(hexti->trigger == IRQ_MODE_TRIG_EDGE_FALLING)
	{
		SP_IRQ_CTRL->polarity[idx] &= ~mask;//low->high
		hexti->trigger |= IRQ_MODE_TRIG_EDGE_ACTIVE;	
	}
	return 1;
}