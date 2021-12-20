/**************************************************************************//**
 * @file     irq_ctrl.h
 * @brief    Interrupt Controller API header file
 * @version  V1.1.0
 * @date     03. March 2020
 ******************************************************************************/
/*
 * Copyright (c) 2017-2020 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if   defined ( __ICCARM__ )
  #pragma system_include         /* treat file as system include file for MISRA check */
#elif defined (__clang__)
  #pragma clang system_header   /* treat file as system include file */
#endif

#ifndef IRQ_CTRL_H_
#define IRQ_CTRL_H_

#include <stdint.h>

#ifndef IRQHANDLER_T
#define IRQHANDLER_T
/// Interrupt handler data type
typedef void (*IRQHandler_t) (void);


#endif

#ifndef IRQN_ID_T
#define IRQN_ID_T
/// Interrupt ID number data type
typedef int32_t IRQn_ID_t;
#endif


#define IRQ_MODE_TRIG_EDGE_RISING   (0x01UL) ///< Trigger: edge triggered interrupt,Q628 only support rasing edge
#define IRQ_MODE_TRIG_EDGE_FALLING  (0x02UL)
#define IRQ_MODE_TRIG_LEVEL_HIGH    (0x04UL) ///< Trigger: high level triggered interrupt
#define IRQ_MODE_TRIG_LEVEL_LOW     (0x08UL) ///< Trigger: low level triggered interrupt
#define IRQ_INVALID					(0x00UL)

#define IRQ_TYPE_IRQ	0x1
#define IRQ_TYPE_FIQ	0x0

/// Initialize interrupt controller.
/// \return 0 on success, -1 on error.
int32_t IRQ_Initialize (void);

/// Register interrupt handler.
/// \param[in]     irqn          interrupt ID number
/// \param[in]     handler       interrupt handler function address
/// \return 0 on success, -1 on error.
int32_t IRQ_SetHandler (IRQn_ID_t irqn, IRQHandler_t handler);

/// Get the registered interrupt handler.
/// \param[in]     irqn          interrupt ID number
/// \return registered interrupt handler function address.
IRQHandler_t IRQ_GetHandler (IRQn_ID_t irqn);

/// Enable interrupt.
/// \param[in]     irqn          interrupt ID number
/// \return 0 on success, -1 on error.
int32_t IRQ_Enable (IRQn_ID_t irqn);

/// Disable interrupt.
/// \param[in]     irqn          interrupt ID number
/// \return 0 on success, -1 on error.
int32_t IRQ_Disable (IRQn_ID_t irqn);

/// Get interrupt enable state.
/// \param[in]     irqn          interrupt ID number
/// \return 0 - interrupt is disabled, 1 - interrupt is enabled.
uint32_t IRQ_GetEnableState (IRQn_ID_t irqn);

/// Configure interrupt request mode.
/// \param[in]     irqn          interrupt ID number
/// \param[in]     mode          mode configuration
/// \return 0 on success, -1 on error.
int32_t IRQ_SetMode (IRQn_ID_t irqn, uint32_t mode);

/// Get interrupt mode configuration.
/// \param[in]     irqn          interrupt ID number
/// \return current interrupt mode configuration with optional IRQ_MODE_ERROR bit set.
uint32_t IRQ_GetMode (IRQn_ID_t irqn);

/// Get ID number of current interrupt request (IRQ).
/// \return interrupt ID number.
IRQn_ID_t IRQ_GetActiveIRQ (void);

/// Get ID number of current fast interrupt request (FIQ).
/// \return interrupt ID number.
IRQn_ID_t IRQ_GetActiveFIQ (void);


/// Clear interrupt flag.
/// \param[in]     irqn          interrupt ID number
/// \return 0 on success, -1 on error.
int32_t IRQ_Clear (IRQn_ID_t irqn);

/// Set interrupt priority value.
/// \param[in]     irqn          interrupt ID number
/// \param[in]     priority      interrupt priority value
/// \return 0 on success, -1 on error.
int32_t IRQ_SetPriority (IRQn_ID_t irqn, uint32_t priority);

/// Get interrupt priority.
/// \param[in]     irqn          interrupt ID number
/// \return current interrupt priority value with optional IRQ_PRIORITY_ERROR bit set.
uint32_t IRQ_GetPriority (IRQn_ID_t irqn);



#endif  // IRQ_CTRL_H_

