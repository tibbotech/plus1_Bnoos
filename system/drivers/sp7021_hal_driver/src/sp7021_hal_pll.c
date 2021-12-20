
#include "sp7021_hal_pll.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t HAL_PLL_GetSystemFreq(void)
{
	if (__HAL_PLL_GET_SYS_BYPASS())
		return HSE_VALUE;

	return ((__HAL_PLL_GET_SYS_NS()+1)*(HSE_VALUE/2))>>(__HAL_PLL_GET_SYS_DIV());

}


uint32_t HAL_PLL_Get_ARM926Freq(void)
{
	
	return (HAL_PLL_GetSystemFreq()>>__HAL_PLL_GET_SYS_ARM926_DIV());
}








#ifdef __cplusplus
}
#endif













