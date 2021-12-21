#ifndef __SP7021_HAL_IPCC_H__
#define __SP7021_HAL_IPCC_H__

#include "sp7021_arm926.h"
#include "sp7021_hal_def.h"
#include "sp70xx.h"
#include "sp7021_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NORMAL_DATA_SIZE	(20)
typedef enum{
	DIRECT_CHN_0 = 0,
	DIRECT_CHN_1 = 1,
	DIRECT_CHN_2 = 2,
	DIRECT_CHN_3 = 3,
	DIRECT_CHN_4 = 4,
	DIRECT_CHN_5 = 5,
	DIRECT_CHN_6 = 6,
	DIRECT_CHN_7 = 7,
	NORMAL_CHN = 8,
	CHN_MAX
}IPPC_CHN_type;

typedef struct
{
	IPCC_Typedef *Instance;     		/*!< Register base address  */
	IRQHandler_t szIRQHDL[CHN_MAX];		/*IPCC MAILBOX Interrupt handle function array*/
} IPCC_HandleTypeDef;
	
HAL_StatusTypeDef HAL_IPCC_Init(IPCC_HandleTypeDef *hipcc);
HAL_StatusTypeDef HAL_IPCC_DeInit(IPCC_HandleTypeDef *hipcc);
HAL_StatusTypeDef HAL_IPCC_UrgentNoify(IPCC_HandleTypeDef *hipcc, IPPC_CHN_type enChn, uint32_t data);
HAL_StatusTypeDef HAL_IPCC_Noify(IPCC_HandleTypeDef *hipcc, uint32_t* pData, uint32_t seek, uint32_t size );





#ifdef __cplusplus
}
#endif

#endif

