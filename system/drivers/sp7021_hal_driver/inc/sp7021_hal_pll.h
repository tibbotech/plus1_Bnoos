#ifndef __sp7021_HAL_PLL_H
#define __sp7021_HAL_PLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sp70xx.h"
#include "sp7021_arm926.h"
#include "sp7021_hal_def.h"


#define __HAL_PLL_GET_SYS_BYPASS() ((uint32_t)((SYS_CLK->pll_ctrl&PLLSYS_BYPASS_Msk)>>PLLSYS_BYPASS_Pos))

#define __HAL_PLL_GET_SYS_NS() ((uint32_t)((SYS_CLK->pll_ctrl&PLLSYS_NS_Msk)>>PLLSYS_NS_Pos))

#define __HAL_PLL_GET_SYS_DIV() ((uint32_t)((SYS_CLK->clk_setting&SYSCLK_DIV_Msk)>>SYSCLK_DIV_Pos))

#define __HAL_PLL_GET_SYS_ARM926_DIV() ((uint32_t)((SYS_CLK->clk_setting&ARM926CLK_DIV_Msk)>>ARM926CLK_DIV_Pos))




#ifdef __cplusplus
}
#endif

#endif



