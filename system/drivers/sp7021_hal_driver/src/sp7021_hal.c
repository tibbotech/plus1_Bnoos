
#include "irq_ctrl.h"
#include "sp7021_arm926.h"
#include "sp7021_hal_def.h"
#include "sp70xx.h"
#include "cmsis_compiler.h"
#include "sp7021_hal_stc.h";

#ifdef __cplusplus
extern "C" {
#endif


#define DEFAULT_SYS_STC_CLK	1000000			//1MHz
__IO uint32_t uwTick = 0;
HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;//1MHz



STC_HandleTypeDef SysStandardTimeClk;



HAL_StatusTypeDef HAL_Init(void)
{
	HAL_InitTick(INIT_STC);
}

HAL_StatusTypeDef HAL_DeInit(void)
{


}

__weak HAL_StatusTypeDef HAL_InitTick (STC_TypeDef *STCx)
{
		SysStandardTimeClk.Instance = STCx;
		SysStandardTimeClk.ClockSource = 0;
		SysStandardTimeClk.ExtDiv = 0;
		/*the 1tick = 1us, 1MHz */	
		SysStandardTimeClk.Prescaler = (HSI_VALUE/DEFAULT_SYS_STC_CLK);
		HAL_STC_Init(&SysStandardTimeClk);
}

void HAL_IncTick(void)
{


}

/*Delay ticks*/
__weak void HAL_Delay(uint32_t Delay)
{
	uint32_t tickstart = HAL_GetTick();
	uint32_t wait = 0;

	if (wait < HAL_MAX_DELAY)
	{
		wait += Delay;
	}

	while ((HAL_GetTick() - tickstart) < wait){
	}
}

uint32_t HAL_GetTick(void)
{
	uint32_t ticks = 0;
	ticks = (uint32_t)HAL_STC_GetCounter(&SysStandardTimeClk);
	return ticks;
}

HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq)
{

}


void HAL_PINMUX_Cfg(PINMUX_Type id, uint32_t pin)
{
	if(pin > 64)
		return;
	if(id >= 0x200)
	{
		/* G3.X*/
		id = id-0x200;
		PINMUX_3_CFG->pinmux_cfg[id/2] = RF_MASK_V((0x7f << ((id%2)*8)), (pin << ((id%2)*8)));
	}
	else if(id >= 0x100)
	{
		/* G2.X*/
		id = id-0x100;
		PINMUX_2_CFG->pinmux_cfg[id/2] = RF_MASK_V((0x7f << ((id%2)*8)), (pin << ((id%2)*8)));
	}
	else if(id == 0x30)
	{
		/* G1.4*/
		PINMUX_1_CFG->pinmux_cfg[3] = RF_MASK_V((0x3 << (id-0x30)), (pin << (id-0x30)));
	}
	else if(id == 0x20)
	{
		/* G1.3*/
		if((id == PINMUX_UPHY1_IF) || (id == PINMUX_PROBE_PORT)|| (id == PINMUX_8051_JTAG))
		{
			PINMUX_1_CFG->pinmux_cfg[2] = RF_MASK_V((0x3 << (id-0x20)), (pin << (id-0x20)));
		}
		else
		{
			PINMUX_1_CFG->pinmux_cfg[2] = RF_MASK_V((1 << (id-0x20)), (pin << (id-0x20)));
		}
	}
	else if(id == 0x10)
	{
		/* G1.2*/
		if((id == PINMUX_UADGB) || (id == PINMUX_UPHY0_IF))
		{
			PINMUX_1_CFG->pinmux_cfg[1] = RF_MASK_V((0x3 << (id-0x10)), (pin << (id-0x10)));
		}
		else
		{
			PINMUX_1_CFG->pinmux_cfg[1] = RF_MASK_V((1 << (id-0x10)), (pin << (id-0x10)));
		}
	}
	else 
	{
		/* G1.1*/
		if((id == PINMUX_SPI_FLASH) || (id == PINMUX_SPI_FLASH_4BIT) || (id == PINMUX_ACHIP_DEBUG)\
			|| (id == PINMUX_ACHIP_UA2AXI)|| (id == PINMUX_HDMI_TX))
		{
			PINMUX_1_CFG->pinmux_cfg[0] = RF_MASK_V((0x3 << id), (pin << id));
		}
		else
		{
			PINMUX_1_CFG->pinmux_cfg[0] = RF_MASK_V((1 << id), (pin << id));
		}
	}

}


void HAL_Module_Clock_enable(MODULE_ID_Type id, uint32_t enable)
{
	if (enable == 1)
		CLK_EN->clock_enable[id/16] = RF_MASK_V_SET(1<<(id%16));
	else
		CLK_EN->clock_enable[id/16] = RF_MASK_V_CLR(1<<(id%16));
}

void HAL_Module_Clock_gate(MODULE_ID_Type id, uint32_t enable)
{
	if (enable == 1)
		CLK_GATE->clock_gate_enable[id/16] = RF_MASK_V_SET(1<<(id%16));
	else
		CLK_GATE->clock_gate_enable[id/16] = RF_MASK_V_CLR(1<<(id%16));
	
}


void HAL_Module_Reset(MODULE_ID_Type id, uint32_t enable)
{
	if (enable == 1)
		MODULE_REST->reset[id/16] = (1<<(id%16)<<16)| (1<<(id%16));
	else
		MODULE_REST->reset[id/16] = (1<<(id%16)<<16)| (0<<(id%16));
	
}

void HAL_lreg(int group)
{
	uint32_t i,  *base;

	base = (uint32_t *)(RF_GRP(group, 0));

	printf("lreg base addr 0x%x\n",base);
	for (i = 0; i < 32; i++) {
		printf( "group %d[%d] = %08x (%d)\n", group, i, base[i], base[i]);
	}
}


#ifdef __cplusplus
}
#endif

