#include "icm.h"

#define TEST_PWM

ICM_InitTypeDef ICM_Init[ICM_NUM];

static int get_instance_index(uint32_t instance)
{
	int index;
	switch(instance)
	{
		case (uint32_t)SP_ICM0:index=ICM0;break;
		case (uint32_t)SP_ICM1:index=ICM1;break;
		case (uint32_t)SP_ICM2:index=ICM2;break;
		case (uint32_t)SP_ICM3:index=ICM3;break;
	}
	return index;
}

void ICM_Initialization()
{
	printf("icm Build @%s, %s\n", __DATE__, __TIME__);
	
	ICM_CTRL_REG* instance;
	int index, muxsel, clksel, eemode, etimes, dtimes, cntscl, tstscl, Pin_data, Pin_clk;
	muxsel = clksel = eemode = etimes = dtimes = cntscl = tstscl = Pin_data = Pin_clk = -1;
	
	//get_para_val();
	muxsel = ICM_MUXSEL_INPUT0;
	clksel = ICM_CLKSEL_27M;
	eemode = ICM_EEMODE_BOTH;
	etimes = 4;
	dtimes = 3;

#if ICM_SEL_INSTANCE == 0
	instance = SP_ICM0;
#elif ICM_SEL_INSTANCE == 1
	instance = SP_ICM1;
#elif ICM_SEL_INSTANCE == 2
	instance = SP_ICM2;
#elif ICM_SEL_INSTANCE == 3
	instance = SP_ICM3;
#endif

	index = get_instance_index((uint32_t)instance);

	ICM_Init[index].index = index;
	ICM_Init[index].instance = instance;

	/* set icm configure */
	HAL_ICM_GetConfig(&ICM_Init[index]);
	if (muxsel != -1) ICM_Init[index].muxsel = muxsel;
	if (clksel != -1) ICM_Init[index].clksel = clksel;
	if (eemode != -1) ICM_Init[index].eemode = eemode;
	if (etimes != -1) ICM_Init[index].etimes = etimes;
	if (dtimes != -1) ICM_Init[index].dtimes = dtimes;
	if (cntscl != -1) ICM_Init[index].cntscl = cntscl;
	if (tstscl != -1) ICM_Init[index].tstscl = tstscl;
	ICM_Init[index].Pin_data = PIN_ICM_DATA;
	ICM_Init[index].Pin_clk = PIN_ICM_CLK;
	HAL_ICM_Init(&ICM_Init[index]);

	/* config interrupt*/
	IRQn_Type irqn = ICM_Init[index].index + 92;
	switch(irqn)
	{
		case ICM0_IRQn:IRQ_SetHandler(ICM0_IRQn, &ICM0_IRQHandler);break;
		case ICM1_IRQn:IRQ_SetHandler(ICM1_IRQn, &ICM1_IRQHandler);break;
		case ICM2_IRQn:IRQ_SetHandler(ICM2_IRQn, &ICM2_IRQHandler);break;
		case ICM3_IRQn:IRQ_SetHandler(ICM3_IRQn, &ICM3_IRQHandler);break;
	}
	IRQ_SetMode(irqn, IRQ_MODE_TRIG_EDGE_RISING);
	IRQ_Enable(irqn);
}

void ICM0_IRQHandler()
{
	HAL_ICM_IRQHandler(&ICM_Init[ICM0]);
}

void ICM1_IRQHandler()
{
	HAL_ICM_IRQHandler(&ICM_Init[ICM1]);
}

void ICM2_IRQHandler()
{
	HAL_ICM_IRQHandler(&ICM_Init[ICM2]);
}

void ICM3_IRQHandler()
{
	HAL_ICM_IRQHandler(&ICM_Init[ICM3]);
}
