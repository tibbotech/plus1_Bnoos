
#ifndef __SP7021_HAL_PWM_H
#define __SP7021_HAL_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp7021_hal_def.h"
#include "sp7021_arm926.h"
#include "sp70xx.h"

typedef enum {
	PWM0=0,
	PWM1,
	PWM2,
	PWM3,
	PWM4,
	PWM5,
	PWM6,
	PWM7,
	PWM_MAX
}PWM_NUMTypeDef;

enum {
	ePWM_DD0=0,
	ePWM_DD1,
	ePWM_DD2,
	ePWM_DD3,
	ePWM_DD_MAX
};


typedef struct
{
  /* set pwm number. the value set from PWM0 to PWM7 */
  PWM_NUMTypeDef pwm_num;
  /* set pwm freq. unit is HZ */
  uint32_t pwm_freq;
  /* set pwm duty. the value set from  1 to 99 */ 
  uint32_t duty_cycle;
  /* set pin number: one of PINMUX_PIN1_00--PINMUX_PIN8_07  */
  uint32_t Pin;
}PWM_InitTypeDef;



#define IS_PWM_NUM_VALID(num)  (((num) >= PWM0) && ((num) <= PWM7))
#define IS_PWM_DUTY_VALID(duty)  (((duty) >= 1) && ((duty) <= 99))

int HAL_PWM_INIT(PWM_InitTypeDef *PWM_Init);
void HAL_PWM_ENABLE(int pwm_num);
void HAL_PWM_DISABLE(int pwm_num);
void HAL_PWM_Period_Set(int pwm_num,uint32_t freq,uint32_t duty_cycle);

#ifdef __cplusplus
}
#endif

#endif
