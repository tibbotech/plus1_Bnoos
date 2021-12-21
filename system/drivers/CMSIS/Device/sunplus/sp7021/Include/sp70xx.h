#ifndef __SP70XX_H
#define __SP70XX_H

#ifdef __cplusplus
 extern "C" {
#endif



typedef enum
{
	RESET = 0,
	SET = !RESET
} FlagStatus;
  



typedef enum
{
	DISABLE = 0,
	ENABLE = !DISABLE
} FunctionalState;


#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

#ifndef TRUE
#define TRUE	1
#define true    1
#endif

#ifndef FALSE
#define FALSE	0
#define false   0
#endif

typedef enum 
{
	ERROR = 0,
	SUCCESS = !ERROR
} ErrorStatus;

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))



#include "sp7021_hal.h"



#ifdef __cplusplus
}
#endif

#endif 

