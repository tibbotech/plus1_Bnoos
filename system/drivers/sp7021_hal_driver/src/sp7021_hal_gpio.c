#include "sp7021_hal_gpio.h"

int gpio_input_invert_val_set(uint32_t bit,int invert_set)
{
	uint32_t idx, value, reg_val;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}

	reg_val = *((volatile unsigned int *)(GPIO_I_INV(idx)));

	if(invert_set)
	{
		value = (reg_val | (1 << (bit & 0x0f) | 1 << ((bit & 0x0f) + 0x10)));
	}
	else
	{
		value = (( reg_val | (1 << ((bit & 0x0f) + 0x10))) & ~( 1 << (bit & 0x0f)));
	}

	*((volatile unsigned int *) (GPIO_I_INV(idx))) = value;	

	return 0;
}


uint32_t gpio_input_invert_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	value = 1 << (bit & 0x0f);
	
	reg_val = *((volatile unsigned int *)(GPIO_I_INV(idx)));

	return ((reg_val & value) ? 1 : 0);
}


int gpio_output_invert_val_set(uint32_t bit,int invert_set)
{
	uint32_t idx, value, reg_val;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}

	reg_val = *((volatile unsigned int *)(GPIO_O_INV(idx)));
	if(invert_set)
	{
		value = (reg_val | (1 << (bit & 0x0f) | 1 << ((bit & 0x0f) + 0x10)));
	}
	else
	{
		value = (( reg_val | (1 << ((bit & 0x0f) + 0x10))) & ~( 1 << (bit & 0x0f)));
	}
	*((volatile unsigned int *) (GPIO_O_INV(idx))) = value;	

	return 0;
}

uint32_t gpio_output_invert_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	value = 1 << (bit & 0x0f);
	
	reg_val = *((volatile unsigned int *)(GPIO_O_INV(idx)));
	return ((reg_val & value) ? 1 : 0);
}

int gpio_open_drain_val_set(uint32_t bit,int od_set)
{
	uint32_t idx, value, reg_val;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}

	reg_val = *((volatile unsigned int *)(GPIO_OD(idx)));

	if(od_set)
	{
		value = (reg_val | (1 << (bit & 0x0f) | 1 << ((bit & 0x0f) + 0x10)));
	}
	else
	{
		value = (( reg_val | (1 << ((bit & 0x0f) + 0x10))) & ~( 1 << (bit & 0x0f)));
	}

	*((volatile unsigned int *) (GPIO_OD(idx))) = value;	

	return 0;
}

uint32_t gpio_open_drain_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	value = 1 << (bit & 0x0f);
	
	reg_val = *((volatile unsigned int *)(GPIO_OD(idx)));
	return ((reg_val & value) ? 1 : 0);
}

/* set first register value , enable
   1: pin set in GPIO mode
   0: pin set in pinmux mode
*/
int gpio_first_val_set(uint32_t bit,int firset_set)
{
	uint32_t idx, value, reg_val;

	idx = bit >> 5;
	if (idx > 4) {
		return -1;
	}

	value = 1 << (bit & 0x1f);

	reg_val = *((volatile unsigned int *)(GPIO_FIRST(idx)));
	if(firset_set)
	{
		reg_val |= value;
	}
	else
	{
		reg_val &= (~value);
	}
	
	*((volatile unsigned int *) (GPIO_FIRST(idx))) = reg_val;

	return 0;
}

uint32_t gpio_first_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 5;
	if (idx > 5) {
		return -1;
	}
	
	value = 1 << (bit & 0x1f);
	
	reg_val = *((volatile unsigned int *)(GPIO_FIRST(idx)));

	return((reg_val & value) ? 1 : 0);
}

/* set master value 
   1: gpio control by gpio register
   0: gpio control by IOP
*/
int gpio_master_val_set(uint32_t bit,int master_set)
{
	uint32_t idx, value, reg_val;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}

	reg_val = *((volatile unsigned int *)(GPIO_MASTER(idx)));

	if(master_set)
	{
		value = (reg_val | (1 << (bit & 0x0f) | 1 << ((bit & 0x0f) + 0x10)));
	}
	else
	{
		value = ((reg_val | (1 << ((bit & 0x0f) + 0x10)) ) & ~( 1 << (bit & 0x0f)));		
	}

	*((volatile unsigned int *) (GPIO_MASTER(idx))) = value;

	return 0;
}

/* get GPIO control mode register value 
   1: GPIO Group control mode 
   0: IOP control mode
*/
uint32_t gpio_master_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}

	value = 1 << (bit & 0x0f);
	reg_val = *((volatile unsigned int *)(GPIO_MASTER(idx)));

	return ((reg_val & value) ? 1 : 0);
}

/* set output enable value 
   1: output mode
   0: input mode
*/
int gpio_oe_val_set(uint32_t bit,int oe_set)
{
	uint32_t idx, value, reg_val;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}

	reg_val = *((volatile unsigned int *)(GPIO_OE(idx)));

	if(oe_set)
	{
		value = (reg_val | (1 << (bit & 0x0f) | 1 << ((bit & 0x0f) + 0x10)));
	}
	else
	{
		value = ((reg_val | (1 << ((bit & 0x0f) + 0x10)) ) & ~( 1 << (bit & 0x0f)));
	}

	*((volatile unsigned int *) (GPIO_OE(idx))) = value;

	return 0;
}

/* get output enable value 
   1: output mode
   0: input mode
*/
uint32_t gpio_oe_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	value = 1 << (bit & 0x0f);
	
	reg_val = *((volatile unsigned int *)(GPIO_OE(idx)));

	return ((reg_val & value) ? 1 : 0);
}

/****** gpio output mode, set output value ****/
int gpio_out_val_set(uint32_t bit, uint32_t gpio_out_value)
{
	uint32_t idx, value, reg_val;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}

	reg_val = *((volatile unsigned int *)(GPIO_OUT(idx)));
	if(gpio_out_value)
	{
		value = (reg_val | (1 << (bit & 0x0f) | 1 << ((bit & 0x0f) + 0x10)));
	}
	else
	{
		value = (( reg_val | (1 << ((bit & 0x0f) + 0x10)) ) & ~( 1 << (bit & 0x0f)));
	}

	*((volatile unsigned int *) (GPIO_OUT(idx))) = value;

	return 0;
}

/****** gpio output mode, get output value ****/
uint32_t gpio_out_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	value = 1 << (bit & 0x0f);
	
	reg_val = *((volatile unsigned int *)(GPIO_OUT(idx)));

	return ((reg_val & value) ? 1 : 0);
}

/****** gpio input mode, get input value ****/
uint32_t gpio_in_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 5;
	if (idx > 5) {
		return -1;
	}
	
	value = 1 << (bit & 0x1f);
	
	reg_val = *((volatile unsigned int *)(GPIO_IN(idx)));

	return ((reg_val & value) ? 1 : 0);
}

uint8_t gpio_is_output(uint32_t GPIO_Pin)
{
	if(gpio_open_drain_val_get(GPIO_Pin) || gpio_oe_val_get(GPIO_Pin))
		return 1;

	return 0;
}

/*****************  HAL interface ************************/
void HAL_GPIO_Init(GPIO_InitTypeDef *GPIO_Init)
{ 

	gpio_assert_param(GPIO_Init);
	gpio_assert_param(IS_GPIO_PIN(GPIO_Init->Pin));

	/* set pin to gpio mode */
	gpio_first_val_set(GPIO_Init->Pin,1);
	/* set pin control by gpio group register */
	gpio_master_val_set(GPIO_Init->Pin,1);

	if(GPIO_Init->opendrain == GPIO_OD_ENABLE)
	{
		/*  pin in opendrain mode */
		gpio_open_drain_val_set(GPIO_Init->Pin,1);
		/* need to invert output in opendrain mode */
		gpio_output_invert_val_set(GPIO_Init->Pin,1);
	}
	else
	{
		/* pin in normal input/ouput mode */
		gpio_oe_val_set(GPIO_Init->Pin,GPIO_Init->Mode);
	}

	/* set pin out default value */
	if((GPIO_Init->Mode == GPIO_OUTPUT_MODE) || (GPIO_Init->opendrain == GPIO_OD_ENABLE))
	{
		gpio_out_val_set(GPIO_Init->Pin,GPIO_Init->default_value);
	}
}

void HAL_GPIO_DeInit(uint16_t GPIO_Pin)
{
	gpio_assert_param(IS_GPIO_PIN(GPIO_Pin));

	/* Disable Selected Pins into GPIO Mode  */
	gpio_first_val_set(GPIO_Pin,0);

	/* Corresponding GPIO is controlled by the correspondingbit of GPIO output enable and GPIO output
	 data registersin this group */
	gpio_master_val_set(GPIO_Pin,1);

	/* Configure pin in non-od status */
	gpio_open_drain_val_set(GPIO_Pin,0);

	/* Corresponding GPIO output value is normal */
	gpio_output_invert_val_set(GPIO_Pin,1);

	/* Corresponding GPIO input value is normal */
	gpio_input_invert_val_set(GPIO_Pin,0);

	/* Corresponding GPIO is in input mode  */
	gpio_oe_val_set(GPIO_Pin,0);

	/* Configure pin output low  */
	gpio_out_val_set(GPIO_Pin,0);

}

GPIO_PinState  HAL_GPIO_ReadPin(uint16_t GPIO_Pin)
{

	GPIO_PinState bitstatus;

	gpio_assert_param(IS_GPIO_PIN(GPIO_Pin));

	if(gpio_is_output(GPIO_Pin))
	{
		bitstatus = gpio_out_val_get(GPIO_Pin)? GPIO_OUT_HIGH:GPIO_OUT_LOW;
	}
	else
	{
		bitstatus = gpio_in_val_get(GPIO_Pin)? GPIO_OUT_HIGH:GPIO_OUT_LOW;
	}
	return bitstatus;
}

void HAL_GPIO_WritePin(uint16_t GPIO_Pin, GPIO_PinState PinState)
{

	gpio_assert_param(IS_GPIO_PIN(GPIO_Pin));
	gpio_assert_param(IS_GPIO_PIN_ACTION(GPIO_Pin));

	/* get input/output mode. write data in output mode only 
		1:output mode  0:input mode 
	*/
	if(gpio_is_output(GPIO_Pin)) 
	{
		gpio_out_val_set(GPIO_Pin,PinState);
	}
}

void HAL_GPIO_TogglePin(uint16_t GPIO_Pin)
{
	gpio_assert_param(IS_GPIO_PIN(GPIO_Pin));
	/* get input/output mode. write data in output mode only 
	   1:output mode  0:input mode 
	*/
	if(gpio_is_output(GPIO_Pin)) 
	{
		gpio_out_val_set(GPIO_Pin,!(gpio_out_val_get(GPIO_Pin)));
	}
}


int HAL_GPIO_Get_Mode(uint16_t GPIO_Pin)
{
	gpio_assert_param(IS_GPIO_PIN(GPIO_Pin));
	/* get input/output mode. write data in output mode only 
	   1:output mode  0:input mode 
	*/
	return gpio_out_val_get(GPIO_Pin);
}
