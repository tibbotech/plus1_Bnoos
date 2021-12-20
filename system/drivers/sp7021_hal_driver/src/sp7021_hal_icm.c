#include "sp7021_hal_icm.h"

void HAL_ICM_IRQHandler(ICM_InitTypeDef *pICM_Init)
{
	uint32_t cnt;
	uint32_t fstate;
	uint32_t pwh;
	uint32_t pwl;
	/* patch:a solution of hareware BUG */
	ICM_Patch(pICM_Init);
	ICM_GetFifoState(pICM_Init, &fstate);
	while( !(fstate & ICM_FEMPTY) )
	{
		ICM_ReadIntrCnt(pICM_Init, &cnt);
		ICM_GetSigPulseWidth(pICM_Init, &pwh, &pwl);
		printf("pwh=%d,pwl=%d,cnt=%d\n", pwh, pwl, cnt);
	}
}

void HAL_ICM_Init(ICM_InitTypeDef *pICM_Init)
{
	HAL_ICM_SetConfig(pICM_Init);

	/* set pinmux must be called after setting clksel*/
	ICM_DATA_SetPinMux(pICM_Init, pICM_Init->Pin_data);
	if ( (pICM_Init->clksel < 4)&&(pICM_Init->Pin_clk != -1) )
	{
		ICM_CLK_SetPinMux(pICM_Init, pICM_Init->Pin_clk);
	}

	ICM_Enable(pICM_Init);
}

void HAL_ICM_GetConfig(ICM_InitTypeDef *pICM_Init)
{
	pICM_Init->muxsel = ICM_GETCFG(0, MUXSEL);
	pICM_Init->clksel = ICM_GETCFG(0, CLKSEL);
	pICM_Init->eemode = ICM_GETCFG(1, EEMODE);
	pICM_Init->etimes = ICM_GETCFG(1, ETIMES);
	pICM_Init->dtimes = ICM_GETCFG(1, DTIMES);
	pICM_Init->cntscl = pICM_Init->instance->cntscl;
	pICM_Init->tstscl = pICM_Init->instance->tstscl;
}

void HAL_ICM_SetConfig(ICM_InitTypeDef *pICM_Init)
{

	ICM_SETCFG(0, MUXSEL, pICM_Init->muxsel);
	ICM_SETCFG(0, CLKSEL, pICM_Init->clksel);
	ICM_SETCFG(1, EEMODE, pICM_Init->eemode);
	ICM_SETCFG(1, ETIMES, pICM_Init->etimes);
	ICM_SETCFG(1, DTIMES, pICM_Init->dtimes);
	pICM_Init->instance->cntscl = pICM_Init->cntscl;
	pICM_Init->instance->tstscl = pICM_Init->tstscl;
}

/* 
  SP7021 have the function of Pin Multiplex. Here set pin for an external interrupt to use
  by configuring the Pinmux control register.
*/
void ICM_DATA_SetPinMux(ICM_InitTypeDef *pICM, int data_pinmux)
{   
	data_pinmux = GPIO_TO_PINMUX(data_pinmux);
	if(IS_VALID_PINMUX(data_pinmux) == 0)
	{
		return -1;
	}
	HAL_PINMUX_Cfg(pICM->index + PINMUX_ICM0_D, data_pinmux);
}

void ICM_CLK_SetPinMux(ICM_InitTypeDef *pICM, int clk_pinmux)
{   
	clk_pinmux = GPIO_TO_PINMUX(clk_pinmux);
	if(IS_VALID_PINMUX(clk_pinmux) == 0)
	{
		return -1;
	}
	HAL_PINMUX_Cfg(pICM->index + PINMUX_ICM0_CLK, clk_pinmux);
}
