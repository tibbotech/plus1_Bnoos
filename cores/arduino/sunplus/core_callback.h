#ifndef __CALLBACK_H
#define __CALLBACK_H
#if defined(CORE_CALLBACK)
#include "variant.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#ifndef CALLBACK_LIST_SIZE
#define CALLBACK_LIST_SIZE  4
#endif

/* Exported functions ------------------------------------------------------- */
void registerCoreCallback(void (*func)(void));
void unregisterCoreCallback(void (*func)(void));
void CoreCallback(void);

#ifdef __cplusplus
}
#endif
#endif // CORE_CALLBACK
#endif /* __CALLBACK_H */

