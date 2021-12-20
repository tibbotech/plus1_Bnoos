#ifndef __SP7021_HAL_GPIO_H
#define __SP7021_HAL_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp7021_hal_def.h"
#include "sp7021_arm926.h"

typedef struct
{
  /* set input/output mode                         GPIO_INPUT_MODE:input          GPIO_OUTPUT_MODE:output   */
  uint8_t Mode;
  /* set OD status. enable OD,the Mode is unused.  GPIO_OD_DISABLE:disable        GPIO_OD_ENABLE:enable     */
  uint8_t opendrain;
  /* set default value.for output mode/enable OD,  GPIO_OUT_LOW:low               GPIO_OUT_HIGH:high        */ 
  uint8_t default_value;
  /* set pin number: one of GPIO_P0_00--GPIO_P8_07  */
  uint32_t Pin;
}GPIO_InitTypeDef;

/** 
  * @brief  GPIO Bit SET and Bit RESET enumeration 
  */
typedef enum
{
  GPIO_OUT_LOW = 0,      // gpio set to low
  GPIO_OUT_HIGH          // gpio set to high
}GPIO_PinState;

typedef enum
{
  GPIO_INPUT_MODE = 0,      // set to input mode
  GPIO_OUTPUT_MODE           // set to output mode
}GPIO_PinMode;

typedef enum
{
  GPIO_OD_DISABLE = 0,      // open drain disable 
  GPIO_OD_ENABLE             // open drain enable 
}GPIO_ODState;


#define IS_GPIO_PIN_ACTION(pin)   gpio_first_val_get(pin)

#define gpio_assert_param(expr)   ((expr) ? (void)0 : (printf("[ERROR]: file %s on line %d\r\n",__FUNCTION__, __LINE__)))


void              HAL_GPIO_Init(GPIO_InitTypeDef *GPIO_Init);
void              HAL_GPIO_DeInit(uint16_t GPIO_Pin);
GPIO_PinState     HAL_GPIO_ReadPin(uint16_t GPIO_Pin);
void              HAL_GPIO_WritePin(uint16_t GPIO_Pin, GPIO_PinState PinState);
void              HAL_GPIO_TogglePin(uint16_t GPIO_Pin);
int               HAL_GPIO_Get_Mode(uint16_t GPIO_Pin);
int gpio_oe_val_set(uint32_t bit,int oe_set);

#ifdef __cplusplus
}
#endif
#endif
