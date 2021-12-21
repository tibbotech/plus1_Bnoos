#ifndef __SP7021_HAL_TIM_H__
#define __SP7021_HAL_TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sp7021_arm926.h"
#include "sp7021_hal_def.h"
#include "irq_ctrl.h"
#include "sp70xx.h"
#include "sp7021_hal.h"

typedef enum {
	CLK_SYS_SRC = 0,				 /* !< System clock is tiger source */
	CLK_STC_SRC,					 /* !< Stand time clock is triger source */	
	CLK_RTC_SRC,					 /* !< Real time clock is triger source */
	CLK_EXT_SRC,					 /* !< External clock is triger source */	
	CLK_SLAVE_TRIG_SRC,				 /* !< The pairs clock is scaled  for triger source */	
	CLK_SLAVE_WRAP_SRC				 /* !< The pairs clock is triger source */
}CLK_SRC;

typedef enum {
	ONE_SHOT_MODE = 0, 				/* !< The counter reaches 0, the timer is stop*/
	REPEAT_MODE 					/* !< The counter reaches 0, the counter will be auto reload the counters */	

}TIM_COUNTER_MODE;


typedef void (*TimCallbackFunc)(void);




/**
  * @brief  TIM Time base Configuration Structure definition
  */
typedef struct
{
	uint32_t ClockSource;		/* !< clock sourcde */
	uint32_t Prescaler;         /*!< Specifies the prescaler value used to divide the TIM clock.
	                               This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF */

	uint32_t Counter;            /*!< Specifies the period value to be loaded into the active
	                               Auto-Reload Register at the next update event.
	                               This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF.  */

	uint32_t ReloadCounter;  /*!< Specifies the repetition counter value. Each time the RCR downcounter*/

	uint32_t AutoReloadPreload;  /*!< Specifies the auto-reload preload.
                                   This parameter can be a value of @ref TIM_AutoReloadPreload */
} TIM_InitTypeDef;


/**
  * @brief  HAL State structures definition
  */
typedef enum
{
	HAL_TIM_STATE_RESET             = 0x00U,    /*!< Peripheral not yet initialized or disabled  */
	HAL_TIM_STATE_READY             = 0x01U,    /*!< Peripheral Initialized and ready for use    */
	HAL_TIM_STATE_BUSY              = 0x02U,    /*!< An internal process is ongoing              */
	HAL_TIM_STATE_TIMEOUT           = 0x03U,    /*!< Timeout state                               */
	HAL_TIM_STATE_ERROR             = 0x04U     /*!< Reception process is ongoing                */
} HAL_TIM_StateTypeDef;


typedef struct
{
	TIM_TypeDef                 *Instance;     /*!< Register base address             */
	TIM_InitTypeDef        Init;          /*!< TIM Time Base required parameters */
	HAL_LockTypeDef             Lock;          /*!< Locking object                    */
	__IO HAL_TIM_StateTypeDef   State;         /*!< TIM operation state               */
	IRQn_Type Irqn;
	IRQHandler_t IrqHandle;
} TIM_HandleTypeDef;


typedef struct  {
  // Those 2 first fields must remain in this order at the beginning of the structure
  void    *__this;
  TIM_HandleTypeDef handle;
  TimCallbackFunc pfcallback;
} timerObj_t;


HAL_StatusTypeDef HAL_TIM_Init(TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_GetState(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_DeInit(TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_GetState(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Start(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Stop(TIM_HandleTypeDef *htim);
timerObj_t *HAL_TIM_Get_timer_obj(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_SetPrescaler(TIM_HandleTypeDef *htim, uint32_t u32Prescaler);
uint32_t HAL_TIM_GetPrescaler(TIM_HandleTypeDef *htim);
uint32_t HAL_TIM_GetCLKSrc(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_setCount(TIM_HandleTypeDef *htim, uint32_t u32Count);
uint32_t HAL_TIM_GetCount(TIM_HandleTypeDef *htim);
uint32_t HAL_TIM_GetMasterCLKFreq(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Enable_Interrupt(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Disable_Interrupt(TIM_HandleTypeDef *htim);










#ifdef __cplusplus
}
#endif

#endif



