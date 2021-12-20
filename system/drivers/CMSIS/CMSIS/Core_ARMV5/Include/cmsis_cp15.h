/**************************************************************************//**
 * @file     cmsis_cp15.h
 * @brief    CMSIS compiler specific macros, functions, instructions
 * @version  V1.0.1
 * @date     07. Sep 2017
 ******************************************************************************/
/*
 * Copyright (c) 2009-2017 ARM Limited. All rights reserved.
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

#ifndef __CMSIS_CP15_H
#define __CMSIS_CP15_H

/** \brief  Get DFSR
    \return               Data Fault Status Register value
 */
__STATIC_FORCEINLINE uint32_t __get_DFSR(void)
{
  uint32_t result;
  __get_CP(15, 0, result, 5, 0, 0);
  return result;
}

/** \brief  Set DFSR
    \param [in]    dfsr  Data Fault Status value to set
 */
__STATIC_FORCEINLINE void __set_DFSR(uint32_t dfsr)
{
  __set_CP(15, 0, dfsr, 5, 0, 0);
}

/** \brief  Get IFSR
    \return               Instruction Fault Status Register value
 */
__STATIC_FORCEINLINE uint32_t __get_IFSR(void)
{
  uint32_t result;
  __get_CP(15, 0, result, 5, 0, 1);
  return result;
}

/** \brief  Set IFSR
    \param [in]    ifsr  Instruction Fault Status value to set
 */
__STATIC_FORCEINLINE void __set_IFSR(uint32_t ifsr)
{
  __set_CP(15, 0, ifsr, 5, 0, 1);
}

/** \brief  Get Cache
    \return Cache Register value
 */
__STATIC_FORCEINLINE uint32_t __get_CACHE(void)
{
  uint32_t result;
  __get_CP(15, 0, result, 0, 0, 1);
  return result;
}



/** \brief  Get TTBR0

    This function returns the value of the Translation Table Base Register 0.

    \return               Translation Table Base Register 0 value
 */
__STATIC_FORCEINLINE uint32_t __get_TTBR0(void)
{
  uint32_t result;
  __get_CP(15, 0, result, 2, 0, 0);
  return result;
}

/** \brief  Set TTBR0

    This function assigns the given value to the Translation Table Base Register 0.

    \param [in]    ttbr0  Translation Table Base Register 0 value to set
 */
__STATIC_FORCEINLINE void __set_TTBR0(uint32_t ttbr0)
{
  __set_CP(15, 0, ttbr0, 2, 0, 0);
}

/** \brief  Get DACR

    This function returns the value of the Domain Access Control Register.

    \return               Domain Access Control Register value
 */
__STATIC_FORCEINLINE uint32_t __get_DACR(void)
{
  uint32_t result;
  __get_CP(15, 0, result, 3, 0, 0);
  return result;
}

/** \brief  Set DACR

    This function assigns the given value to the Domain Access Control Register.

    \param [in]    dacr   Domain Access Control Register value to set
 */
__STATIC_FORCEINLINE void __set_DACR(uint32_t dacr)
{
  __set_CP(15, 0, dacr, 3, 0, 0);
}

/** \brief  Set SCTLR

    This function assigns the given value to the System Control Register.

    \param [in]    sctlr  System Control Register value to set
 */
__STATIC_FORCEINLINE void __set_SCTLR(uint32_t sctlr)
{
  __set_CP(15, 0, sctlr, 1, 0, 0);
}

/** \brief  Get SCTLR
    \return               System Control Register value
 */
__STATIC_FORCEINLINE uint32_t __get_SCTLR(void)
{
  uint32_t result;
  __get_CP(15, 0, result, 1, 0, 0);
  return result;
}



/** \brief  Set TLBIALL

  TLB Invalidate All
 */
__STATIC_FORCEINLINE void __set_TLBIALL(uint32_t value)
{
  __set_CP(15, 0, value, 8, 7, 0);
}


/** \brief  Set ICIALLU

  Instruction Cache Invalidate All
 */
__STATIC_FORCEINLINE void __set_ICIALLU(uint32_t value)
{
  __set_CP(15, 0, value, 7, 5, 0);
  __ASM volatile ("nop");
  __ASM volatile ("nop");
  __ASM volatile ("nop");
  __ASM volatile ("nop");
  __ASM volatile ("nop");
  __ASM volatile ("nop");
}

/** \brief  Set DCCMVAC

  Data cache clean
 */
__STATIC_FORCEINLINE void __set_DCCMVAC(uint32_t value)
{
  __set_CP(15, 0, value, 7, 10, 1);
}

/** \brief  Set DCIMVAC

  Data cache invalidate
 */
__STATIC_FORCEINLINE void __set_DCIMVAC(uint32_t value)
{
  __set_CP(15, 0, value, 7, 6, 1);
}

/** \brief  Set DCCIMVAC

  Data cache clean and invalidate
 */
__STATIC_FORCEINLINE void __set_DCCIMVAC(uint32_t value)
{
  __set_CP(15, 0, value, 7, 14, 1);
}


/** \brief  Set DCISW
 */
__STATIC_FORCEINLINE void __set_DCISW(uint32_t value)
{
  __set_CP(15, 0, value, 7, 6, 2);
}

/** \brief  Set DCCSW
 */
__STATIC_FORCEINLINE void __set_DCCSW(uint32_t value)
{
  __set_CP(15, 0, value, 7, 10, 2);
}

/** \brief  Set DCCISW
 */
__STATIC_FORCEINLINE void __set_DCCISW(uint32_t value)
{
  __set_CP(15, 0, value, 7, 14, 2);
}

#endif

