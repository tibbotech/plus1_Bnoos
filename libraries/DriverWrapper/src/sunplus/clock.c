


#include "clock.h"
#include "sp7021_hal.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
  * @brief  Function called to read the current micro second
  * @param  None
  * @retval None
  */
uint32_t getCurrentMicros(void)
{
	return HAL_GetTick();
}

/**
  * @brief  Function called wto read the current millisecond
  * @param  None
  * @retval None
  */
uint32_t getCurrentMillis(void)
{
	return HAL_GetTick()/1000;
}



#ifdef __cplusplus
}
#endif


