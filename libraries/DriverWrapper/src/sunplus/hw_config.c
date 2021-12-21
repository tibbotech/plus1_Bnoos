#include "hw_config.h"



#ifdef __cplusplus
extern "C" {
#endif


void hw_config_init(void)
{

  /* Initialize the HAL */
  HAL_Init();

#if 0
  /* Configure the system clock */
  SystemClock_Config();
#endif

}



#ifdef __cplusplus
}
#endif






