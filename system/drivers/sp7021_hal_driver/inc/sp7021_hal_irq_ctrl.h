#ifndef __SP7021_HAL_IRQ_CTRL_H_
#define __SP7021_HAL_IRQ_CTRL_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "irq_ctrl.h"
#include "sp7021_arm926.h"
#include "sp7021_hal_def.h"
#include "sp70xx.h"
#include "cmsis_compiler.h"
#if 0
void RESET_HANDLE( void ) __attribute__ ((naked));
void UNDEF_INS_HANDLE( void ) __attribute__ ((naked));
void SWI_HANDLE( void ) __attribute__ ((naked));
void PREFE_ABORT_HANDLE( void ) __attribute__ ((naked));
void DATA_ABORT_HANDLE( void ) __attribute__ ((naked));
void NO_USED_HANDLE( void ) __attribute__ ((naked));
#endif
void IRQ_HANDLE( void ) __attribute__ ((naked));
void FIQ_HANDLE( void ) __attribute__ ((naked));

#ifdef __cplusplus
}
#endif

#endif 
