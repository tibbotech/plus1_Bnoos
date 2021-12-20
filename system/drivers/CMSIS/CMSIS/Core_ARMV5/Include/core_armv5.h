/**************************************************************************//**
 * @file     core_v5.h
 * @brief    CMSIS Cortex-A Core Peripheral Access Layer Header File
 * @version  V0.0.1
 * @date     30. Mar 2020
 ******************************************************************************/
/*
 * Copyright (c) 2009-2020 ARM Limited. All rights reserved.
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

#ifndef __CORE_ARMV5_H_GENERIC
#define __CORE_ARMV5_H_GENERIC

#ifdef __cplusplus
 extern "C" {
#endif

/*******************************************************************************
 *                 CMSIS definitions
 ******************************************************************************/

/*  CMSIS CA definitions */
#define __CA_CMSIS_VERSION_MAIN  (1U)                                      /*!< \brief [31:16] CMSIS-Core(A) main version   */
#define __CA_CMSIS_VERSION_SUB   (1U)                                      /*!< \brief [15:0]  CMSIS-Core(A) sub version    */
#define __CA_CMSIS_VERSION       ((__CA_CMSIS_VERSION_MAIN << 16U) | \
                                   __CA_CMSIS_VERSION_SUB          )       /*!< \brief CMSIS-Core(A) version number         */

#if defined ( __CC_ARM )
  #if defined __TARGET_FPU_VFP
    #if (__FPU_PRESENT == 1)
      #define __FPU_USED       1U
    #else
      #warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED       0U
    #endif
  #else
    #define __FPU_USED         0U
  #endif

#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #if defined __ARM_FP
    #if defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)
      #define __FPU_USED       1U
    #else
      #warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED       0U
    #endif
  #else
    #define __FPU_USED         0U
  #endif

#elif defined ( __ICCARM__ )
  #if defined __ARMVFP__
    #if (__FPU_PRESENT == 1)
      #define __FPU_USED       1U
    #else
      #warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED       0U
    #endif
  #else
    #define __FPU_USED         0U
  #endif

#elif defined ( __TMS470__ )
  #if defined __TI_VFP_SUPPORT__
    #if (__FPU_PRESENT == 1)
      #define __FPU_USED       1U
    #else
      #warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED       0U
    #endif
  #else
    #define __FPU_USED         0U
  #endif

#elif defined ( __GNUC__ )
  #if defined (__VFP_FP__) && !defined(__SOFTFP__)
    #if (__FPU_PRESENT == 1)
      #define __FPU_USED       1U
    #else
      #warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED       0U
    #endif
  #else
    #define __FPU_USED         0U
  #endif

#elif defined ( __TASKING__ )
  #if defined __FPU_VFP__
    #if (__FPU_PRESENT == 1)
      #define __FPU_USED       1U
    #else
      #error "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED       0U
    #endif
  #else
    #define __FPU_USED         0U
  #endif
#endif

#include "cmsis_compiler.h"               /* CMSIS compiler specific defines */

#ifdef __cplusplus
}
#endif

#endif /* __CORE_CA_H_GENERIC */

#ifndef __CMSIS_GENERIC

#ifndef __CORE_CA_H_DEPENDANT
#define __CORE_CA_H_DEPENDANT

#ifdef __cplusplus
 extern "C" {
#endif

 /* check device defines and use defaults */
#if defined __CHECK_DEVICE_DEFINES
  #ifndef __CA_REV
    #define __CA_REV              0x0000U
    #warning "__CA_REV not defined in device header file; using default!"
  #endif
  
  #ifndef __FPU_PRESENT
    #define __FPU_PRESENT             0U
    #warning "__FPU_PRESENT not defined in device header file; using default!"
  #endif
    
  #ifndef __GIC_PRESENT
    #define __GIC_PRESENT             1U
    #warning "__GIC_PRESENT not defined in device header file; using default!"
  #endif
  
  #ifndef __TIM_PRESENT
    #define __TIM_PRESENT             1U
    #warning "__TIM_PRESENT not defined in device header file; using default!"
  #endif
  
  #ifndef __L2C_PRESENT
    #define __L2C_PRESENT             0U
    #warning "__L2C_PRESENT not defined in device header file; using default!"
  #endif
#endif

/* IO definitions (access restrictions to peripheral registers) */
#ifdef __cplusplus
  #define   __I     volatile             /*!< \brief Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< \brief Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< \brief Defines 'write only' permissions */
#define     __IO    volatile             /*!< \brief Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*!< \brief Defines 'read only' structure member permissions */
#define     __OM     volatile            /*!< \brief Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*!< \brief Defines 'read / write' structure member permissions */
#define RESERVED(N, T) T RESERVED##N;    // placeholder struct members used for "reserved" areas


#define HAL_DCACHE_LINE_SIZE            32
#define HAL_DCACHE_SIZE					(16*1024)
#define HAL_DCACHE_WAYS					4
#define HAL_DCACHE_SETS 				(HAL_DCACHE_SIZE/(HAL_DCACHE_LINE_SIZE*HAL_DCACHE_WAYS))


#define HAL_ICACHE_LINE_SIZE            32
#define HAL_ICACHE_SIZE					(16*1024)
#define HAL_ICACHE_WAYS					4


 /*******************************************************************************
  *                 Register Abstraction
   Core Register contain:
   - CPSR
   - CP15 Registers
   - L2C-310 Cache Controller
   - Generic Interrupt Controller Distributor
   - Generic Interrupt Controller Interface
  ******************************************************************************/

/* Core Register CPSR */
typedef union
{
  struct
  {
    uint32_t M:5;                        /*!< \brief bit:  0.. 4  Mode field */
    uint32_t T:1;                        /*!< \brief bit:      5  Thumb execution state bit */
    uint32_t F:1;                        /*!< \brief bit:      6  FIQ mask bit */
    uint32_t I:1;                        /*!< \brief bit:      7  IRQ mask bit */
    uint32_t A:1;                        /*!< \brief bit:      8  Asynchronous abort mask bit */
    uint32_t E:1;                        /*!< \brief bit:      9  Endianness execution state bit */
    uint32_t IT1:6;                      /*!< \brief bit: 10..15  If-Then execution state bits 2-7 */
    uint32_t GE:4;                       /*!< \brief bit: 16..19  Greater than or Equal flags */
    RESERVED(0:4, uint32_t)              
    uint32_t J:1;                        /*!< \brief bit:     24  Jazelle bit */
    uint32_t IT0:2;                      /*!< \brief bit: 25..26  If-Then execution state bits 0-1 */
    uint32_t Q:1;                        /*!< \brief bit:     27  Saturation condition flag */
    uint32_t V:1;                        /*!< \brief bit:     28  Overflow condition code flag */
    uint32_t C:1;                        /*!< \brief bit:     29  Carry condition code flag */
    uint32_t Z:1;                        /*!< \brief bit:     30  Zero condition code flag */
    uint32_t N:1;                        /*!< \brief bit:     31  Negative condition code flag */
  } b;                                   /*!< \brief Structure used for bit  access */
  uint32_t w;                            /*!< \brief Type      used for word access */
} CPSR_Type;



/* CPSR Register Definitions */
#define CPSR_N_Pos                       31U                                    /*!< \brief CPSR: N Position */
#define CPSR_N_Msk                       (1UL << CPSR_N_Pos)                    /*!< \brief CPSR: N Mask */

#define CPSR_Z_Pos                       30U                                    /*!< \brief CPSR: Z Position */
#define CPSR_Z_Msk                       (1UL << CPSR_Z_Pos)                    /*!< \brief CPSR: Z Mask */

#define CPSR_C_Pos                       29U                                    /*!< \brief CPSR: C Position */
#define CPSR_C_Msk                       (1UL << CPSR_C_Pos)                    /*!< \brief CPSR: C Mask */

#define CPSR_V_Pos                       28U                                    /*!< \brief CPSR: V Position */
#define CPSR_V_Msk                       (1UL << CPSR_V_Pos)                    /*!< \brief CPSR: V Mask */

#define CPSR_Q_Pos                       27U                                    /*!< \brief CPSR: Q Position */
#define CPSR_Q_Msk                       (1UL << CPSR_Q_Pos)                    /*!< \brief CPSR: Q Mask */

#define CPSR_IT0_Pos                     25U                                    /*!< \brief CPSR: IT0 Position */
#define CPSR_IT0_Msk                     (3UL << CPSR_IT0_Pos)                  /*!< \brief CPSR: IT0 Mask */

#define CPSR_J_Pos                       24U                                    /*!< \brief CPSR: J Position */
#define CPSR_J_Msk                       (1UL << CPSR_J_Pos)                    /*!< \brief CPSR: J Mask */

#define CPSR_GE_Pos                      16U                                    /*!< \brief CPSR: GE Position */
#define CPSR_GE_Msk                      (0xFUL << CPSR_GE_Pos)                 /*!< \brief CPSR: GE Mask */

#define CPSR_IT1_Pos                     10U                                    /*!< \brief CPSR: IT1 Position */
#define CPSR_IT1_Msk                     (0x3FUL << CPSR_IT1_Pos)               /*!< \brief CPSR: IT1 Mask */

#define CPSR_E_Pos                       9U                                     /*!< \brief CPSR: E Position */
#define CPSR_E_Msk                       (1UL << CPSR_E_Pos)                    /*!< \brief CPSR: E Mask */

#define CPSR_A_Pos                       8U                                     /*!< \brief CPSR: A Position */
#define CPSR_A_Msk                       (1UL << CPSR_A_Pos)                    /*!< \brief CPSR: A Mask */

#define CPSR_I_Pos                       7U                                     /*!< \brief CPSR: I Position */
#define CPSR_I_Msk                       (1UL << CPSR_I_Pos)                    /*!< \brief CPSR: I Mask */

#define CPSR_F_Pos                       6U                                     /*!< \brief CPSR: F Position */
#define CPSR_F_Msk                       (1UL << CPSR_F_Pos)                    /*!< \brief CPSR: F Mask */

#define CPSR_T_Pos                       5U                                     /*!< \brief CPSR: T Position */
#define CPSR_T_Msk                       (1UL << CPSR_T_Pos)                    /*!< \brief CPSR: T Mask */

#define CPSR_M_Pos                       0U                                     /*!< \brief CPSR: M Position */
#define CPSR_M_Msk                       (0x1FUL << CPSR_M_Pos)                 /*!< \brief CPSR: M Mask */

#define CPSR_M_USR                       0x10U                                  /*!< \brief CPSR: M User mode (PL0) */
#define CPSR_M_FIQ                       0x11U                                  /*!< \brief CPSR: M Fast Interrupt mode (PL1) */
#define CPSR_M_IRQ                       0x12U                                  /*!< \brief CPSR: M Interrupt mode (PL1) */
#define CPSR_M_SVC                       0x13U                                  /*!< \brief CPSR: M Supervisor mode (PL1) */
#define CPSR_M_MON                       0x16U                                  /*!< \brief CPSR: M Monitor mode (PL1) */
#define CPSR_M_ABT                       0x17U                                  /*!< \brief CPSR: M Abort mode (PL1) */
#define CPSR_M_HYP                       0x1AU                                  /*!< \brief CPSR: M Hypervisor mode (PL2) */
#define CPSR_M_UND                       0x1BU                                  /*!< \brief CPSR: M Undefined mode (PL1) */
#define CPSR_M_SYS                       0x1FU                                  /*!< \brief CPSR: M System mode (PL1) */

/* CP15 Register SCTLR */
typedef union
{
  struct
  {
    uint32_t M:1;                        /*!< \brief bit:     0  MMU enable */
    uint32_t A:1;                        /*!< \brief bit:     1  Alignment check enable */
    uint32_t C:1;                        /*!< \brief bit:     2  Cache enable */
    RESERVED(0:3, uint32_t)             
    uint32_t B:1;                        /*!< \brief bit:     7  Endianness model */
	uint32_t S:1;                        /*!< \brief bit:     8  system protect  */
	uint32_t R:1;                        /*!< \brief bit:     9  ROM protect  */
    RESERVED(1:2, uint32_t)              
    uint32_t I:1;                        /*!< \brief bit:    12  Instruction cache enable */
    uint32_t V:1;                        /*!< \brief bit:    13  Vectors bit */
    uint32_t RR:1;                       /*!< \brief bit:    14  Round Robin select */
	uint32_t L4:1;                       /*!< \brief bit:    15  L4 set the T Flag  */
    RESERVED(2:16, uint32_t)              
  } b;                                   /*!< \brief Structure used for bit  access */
  uint32_t w;                            /*!< \brief Type      used for word access */
} SCTLR_Type;


#define SCTLR_L4_Pos                     15U                                    /*!< \brief SCTLR: HA Position */
#define SCTLR_L4_Msk                     (1UL << SCTLR_L4_Pos)                  /*!< \brief SCTLR: HA Mask */

#define SCTLR_RR_Pos                     14U                                    /*!< \brief SCTLR: RR Position */
#define SCTLR_RR_Msk                     (1UL << SCTLR_RR_Pos)                  /*!< \brief SCTLR: RR Mask */

#define SCTLR_V_Pos                      13U                                    /*!< \brief SCTLR: V Position */
#define SCTLR_V_Msk                      (1UL << SCTLR_V_Pos)                   /*!< \brief SCTLR: V Mask */

#define SCTLR_I_Pos                      12U                                    /*!< \brief SCTLR: I Position */
#define SCTLR_I_Msk                      (1UL << SCTLR_I_Pos)                   /*!< \brief SCTLR: I Mask */

#define SCTLR_R_Pos                      9U                                    /*!< \brief SCTLR: Z Position */
#define SCTLR_R_Msk                      (1UL << SCTLR_R_Pos)                   /*!< \brief SCTLR: Z Mask */

#define SCTLR_S_Pos                     8U                                    /*!< \brief SCTLR: SW Position */
#define SCTLR_S_Msk                     (1UL << SCTLR_S_Pos)                  /*!< \brief SCTLR: SW Mask */

#define SCTLR_B_Pos                      7U                                     /*!< \brief SCTLR: B Position */
#define SCTLR_B_Msk                      (1UL << SCTLR_B_Pos)                   /*!< \brief SCTLR: B Mask */

#define SCTLR_C_Pos                      2U                                     /*!< \brief SCTLR: C Position */
#define SCTLR_C_Msk                      (1UL << SCTLR_C_Pos)                   /*!< \brief SCTLR: C Mask */

#define SCTLR_A_Pos                      1U                                     /*!< \brief SCTLR: A Position */
#define SCTLR_A_Msk                      (1UL << SCTLR_A_Pos)                   /*!< \brief SCTLR: A Mask */

#define SCTLR_M_Pos                      0U                                     /*!< \brief SCTLR: M Position */
#define SCTLR_M_Msk                      (1UL << SCTLR_M_Pos)                   /*!< \brief SCTLR: M Mask */

/* CP15 Register DFSR */
typedef union
{
  struct
  {
    uint32_t FS0:4;                      /*!< \brief bit: 0.. 3  Fault Status bits bit 0-3 */
    uint32_t Domain:4;                   /*!< \brief bit: 4.. 7  Fault on which domain */
    RESERVED(0:16, uint32_t)              
  }b;
  uint32_t w;                            /*!< \brief Type      used for word access */
} DFSR_Type;

#define DFSR_Domain_Pos                  4U                                     /*!< \brief DFSR: Domain Position */
#define DFSR_Domain_Msk                  (0xFUL << DFSR_Domain_Pos)             /*!< \brief DFSR: Domain Mask */

#define DFSR_FS0_Pos                     0U                                     /*!< \brief DFSR: FS0 Position */
#define DFSR_FS0_Msk                     (0xFUL << DFSR_FS0_Pos)                /*!< \brief DFSR: FS0 Mask */

#define DFSR_STATUS_Pos                  0U                                     /*!< \brief DFSR: STATUS Position */
#define DFSR_STATUS_Msk                  (0x3FUL << DFSR_STATUS_Pos)            /*!< \brief DFSR: STATUS Mask */

/* CP15 Register IFSR */
typedef union
{
  struct
  {
    uint32_t FS0:4;                      /*!< \brief bit: 0.. 3  Fault Status bits bit 0-3 */
    uint32_t Domain:4;             		 /*!< \brief bit: 4.. 7  Fault on which domain */
    RESERVED(0:16, uint32_t)            
  }b;                                   /*!< \brief Structure used for bit access in short format */
  uint32_t w;                            /*!< \brief Type      used for word access */
} IFSR_Type;

#define IFSR_Domain_Pos                  4U                                     /*!< \brief DFSR: Domain Position */
#define IFSR_Domain_Msk                  (0xFUL << IFSR_Domain_Pos)             /*!< \brief DFSR: Domain Mask */

#define IFSR_FS0_Pos                     0U                                     /*!< \brief IFSR: FS0 Position */
#define IFSR_FS0_Msk                     (0xFUL << IFSR_FS0_Pos)                /*!< \brief IFSR: FS0 Mask */

#define IFSR_STATUS_Pos                  0U                                     /*!< \brief IFSR: STATUS Position */
#define IFSR_STATUS_Msk                  (0x3FUL << IFSR_STATUS_Pos)            /*!< \brief IFSR: STATUS Mask */

/* CP15 Register ISR */
typedef union
{
  struct
  {
    RESERVED(0:6, uint32_t)              
    uint32_t F:1;                        /*!< \brief bit:     6  FIQ pending bit */
    uint32_t I:1;                        /*!< \brief bit:     7  IRQ pending bit */
    uint32_t A:1;                        /*!< \brief bit:     8  External abort pending bit */
    RESERVED(1:23, uint32_t)             
  } b;                                   /*!< \brief Structure used for bit  access */
  uint32_t w;                            /*!< \brief Type      used for word access */
} ISR_Type;

#define ISR_A_Pos                        13U                                    /*!< \brief ISR: A Position */
#define ISR_A_Msk                        (1UL << ISR_A_Pos)                     /*!< \brief ISR: A Mask */

#define ISR_I_Pos                        12U                                    /*!< \brief ISR: I Position */
#define ISR_I_Msk                        (1UL << ISR_I_Pos)                     /*!< \brief ISR: I Mask */

#define ISR_F_Pos                        11U                                    /*!< \brief ISR: F Position */
#define ISR_F_Msk                        (1UL << ISR_F_Pos)                     /*!< \brief ISR: F Mask */

/* DACR Register */
#define DACR_D_Pos_(n)                   (2U*n)                                 /*!< \brief DACR: Dn Position */
#define DACR_D_Msk_(n)                   (3UL << DACR_D_Pos_(n))                /*!< \brief DACR: Dn Mask */
#define DACR_Dn_NOACCESS                 0U                                     /*!< \brief DACR Dn field: No access */
#define DACR_Dn_CLIENT                   1U                                     /*!< \brief DACR Dn field: Client */
#define DACR_Dn_MANAGER                  3U                                     /*!< \brief DACR Dn field: Manager */

/**
  \brief     Mask and shift a bit field value for use in a register bit range.
  \param [in] field  Name of the register bit field.
  \param [in] value  Value of the bit field. This parameter is interpreted as an uint32_t type.
  \return           Masked and shifted value.
*/
#define _VAL2FLD(field, value)    (((uint32_t)(value) << field ## _Pos) & field ## _Msk)

/**
  \brief     Mask and shift a register value to extract a bit filed value.
  \param [in] field  Name of the register bit field.
  \param [in] value  Value of register. This parameter is interpreted as an uint32_t type.
  \return           Masked and shifted bit field value.
*/
#define _FLD2VAL(field, value)    (((uint32_t)(value) & field ## _Msk) >> field ## _Pos)


/**
 \brief  Union type to access the L2C_310 Cache Controller.
*/



 /*******************************************************************************
  *                Hardware Abstraction Layer
   Core Function Interface contains:
   - L1 Cache Functions
   - L2C-310 Cache Controller Functions 
   - PL1 Timer Functions
   - GIC Functions
   - MMU Functions
  ******************************************************************************/
 
/* ##########################  L1 Cache functions  ################################# */

/** \brief Enable Caches by setting I and C bits in SCTLR register.
*/
__STATIC_FORCEINLINE void L1C_EnableCaches(void) {
  __set_SCTLR( __get_SCTLR() | SCTLR_I_Msk | SCTLR_C_Msk);
  __ISB();
}

/** \brief Disable Caches by clearing I and C bits in SCTLR register.
*/
__STATIC_FORCEINLINE void L1C_DisableCaches(void) {
  __set_SCTLR( __get_SCTLR() & (~SCTLR_I_Msk) & (~SCTLR_C_Msk));
  __ISB();
}


/** \brief  Invalidate the whole instruction cache
*/
__STATIC_FORCEINLINE void L1C_InvalidateICacheAll(void) {
  __set_ICIALLU(0);
  __DSB();     //ensure completion of the invalidation
  __ISB();     //ensure instruction fetch path sees new I cache state
}

/** \brief  Clean data cache line by address.
* \param [in] va Pointer to data to clear the cache for.
*/
__STATIC_FORCEINLINE void L1C_CleanDCacheMVA(void *va) {
  __set_DCCMVAC((uint32_t)va);
  __DMB();     //ensure the ordering of data cache maintenance operations and their effects
}

/** \brief  Invalidate data cache line by address.
* \param [in] va Pointer to data to invalidate the cache for.
*/
__STATIC_FORCEINLINE void L1C_InvalidateDCacheMVA(void *va) {
  __set_DCIMVAC((uint32_t)va);
  __DMB();     //ensure the ordering of data cache maintenance operations and their effects
}

/** \brief  Clean and Invalidate data cache by address.
* \param [in] va Pointer to data to invalidate the cache for.
*/
__STATIC_FORCEINLINE void L1C_CleanInvalidateDCacheMVA(void *va) {
  __set_DCCIMVAC((uint32_t)va);
  __DMB();     //ensure the ordering of data cache maintenance operations and their effects
}

/** \brief Calculate log2 rounded up
*  - log(0)  => 0
*  - log(1)  => 0
*  - log(2)  => 1
*  - log(3)  => 2
*  - log(4)  => 2
*  - log(5)  => 3
*        :      :
*  - log(16) => 4
*  - log(32) => 5
*        :      :
* \param [in] n input value parameter 
* \return log2(n)
*/
__STATIC_FORCEINLINE uint8_t __log2_up(uint32_t n)
{
  if (n < 2U) {
    return 0U;
  }
  uint8_t log = 0U;
  uint32_t t = n;
  while(t > 1U)
  {
    log++;
    t >>= 1U;
  }
  if (n & 1U) { log++; }
  return log;
}


#if 0
/** \brief  Apply cache maintenance to given cache level.
* \param [in] maint 0 - invalidate, 1 - clean, otherwise - invalidate and clean
*/
__STATIC_FORCEINLINE void __L1C_MaintainDCacheSetWay(uint32_t level, uint32_t maint)
{

#if 0
  uint32_t Dummy;
  uint32_t ccsidr;
  uint32_t num_sets;
  uint32_t num_ways;
  uint32_t shift_way;
  uint32_t log2_linesize;
   int32_t log2_num_ways;

  Dummy = level << 1U;
  /* set csselr, select ccsidr register */
  __set_CSSELR(Dummy);
  /* get current ccsidr register */
  ccsidr = __get_CCSIDR();
  num_sets = ((ccsidr & 0x0FFFE000U) >> 13U) + 1U;
  num_ways = ((ccsidr & 0x00001FF8U) >> 3U) + 1U;
  log2_linesize = (ccsidr & 0x00000007U) + 2U + 2U;
  log2_num_ways = __log2_up(num_ways);
  if ((log2_num_ways < 0) || (log2_num_ways > 32)) {
    return; // FATAL ERROR
  }
  shift_way = 32U - (uint32_t)log2_num_ways;
  for(int32_t way = num_ways-1; way >= 0; way--)
  {
    for(int32_t set = num_sets-1; set >= 0; set--)
    {
      Dummy = (level << 1U) | (((uint32_t)set) << log2_linesize) | (((uint32_t)way) << shift_way);
      switch (maint)
      {
        case 0U: __set_DCISW(Dummy);  break;
        case 1U: __set_DCCSW(Dummy);  break;
        default: __set_DCCISW(Dummy); break;
      }
    }
  }
  __DMB();
#endif  
}
#endif

#if 0
/** \brief  Clean and Invalidate the entire data or unified cache
* Generic mechanism for cleaning/invalidating the entire data or unified cache to the point of coherency
* \param [in] op 0 - invalidate, 1 - clean, otherwise - invalidate and clean
*/
__STATIC_FORCEINLINE void L1C_CleanInvalidateCache(uint32_t op) {

#if 0
  uint32_t clidr;
  uint32_t cache_type;
  clidr =  __get_CLIDR();
  for(uint32_t i = 0U; i<7U; i++)
  {
    cache_type = (clidr >> i*3U) & 0x7UL;
    if ((cache_type >= 2U) && (cache_type <= 4U))
    {
      __L1C_MaintainDCacheSetWay(i, op);
    }
  }
#endif
}
#endif

#if 0

/** \brief  Clean and Invalidate the entire data or unified cache
* Generic mechanism for cleaning/invalidating the entire data or unified cache to the point of coherency
* \param [in] op 0 - invalidate, 1 - clean, otherwise - invalidate and clean
* \deprecated Use generic L1C_CleanInvalidateCache instead.
*/
CMSIS_DEPRECATED
__STATIC_FORCEINLINE void __L1C_CleanInvalidateCache(uint32_t op) {
  L1C_CleanInvalidateCache(op);
}
#endif
/** \brief  Invalidate the whole data cache.
*/
__STATIC_FORCEINLINE void L1C_InvalidateDCacheAll(void) {

	uint32_t value;
	__set_CP(15, 0, value, 7, 6, 0);
	__set_CP(15, 0, value, 8, 7, 0);
  
}

#if 0

/** \brief  Clean the whole data cache.
 */
__STATIC_FORCEINLINE void L1C_CleanDCacheAll(void) {
  L1C_CleanInvalidateCache(1);
}
#endif


#if 0
/** \brief  Clean and invalidate the whole data cache.
 */
__STATIC_FORCEINLINE void L1C_CleanInvalidateDCacheAll(void) {
  L1C_CleanInvalidateCache(2);
}
#endif



__STATIC_FORCEINLINE void L1C_InvlidateDCache(uint32_t base, uint32_t size)
{
	uint32_t end = 0;
	uint32_t addr = 0;
	uint32_t value = 0;

	for ( addr = (~(HAL_DCACHE_LINE_SIZE - 1)) & base, end = addr + size;						   
			 addr < end ;												   
			 addr += HAL_DCACHE_LINE_SIZE )  

	
	{
		__set_DCIMVAC(addr);
	}

}

// Write dirty cache lines to memory for the given address range.
__STATIC_FORCEINLINE void L1C_CacheStore(uint32_t base, uint32_t size)
{
	
	uint32_t end = 0;
	uint32_t addr = 0;
	uint32_t value = 0;

	for ( addr = (~(HAL_DCACHE_LINE_SIZE - 1)) & base, end = addr + size;						   
			 addr < end ;												   
			 addr += HAL_DCACHE_LINE_SIZE )  
	{
		__set_DCCMVAC(addr);
	}


	__set_CP(15, 0, value, 7, 10, 4);

}

__STATIC_FORCEINLINE void L1C_DCacheFlush(uint32_t base, uint32_t size)
{
	L1C_CacheStore(base, size);
	L1C_InvlidateDCache(base, size);
}

__STATIC_FORCEINLINE void L1C_DCacheSync(void)
{
	 __ASM volatile (                                                      \
        "1: "                   /* clean & invalidate D index */ \
        "mrc    p15, 0, r15, c7, c14, 3;" \
        "bne    1b;" \
        "mcr    p15,0,r0,c7,c10,4;" /* drain the write buffer */        \
        : \
        : \
        : "r0" /* Clobber list */                                       \
        );           

}



// -------------------------------------------------------------------------
// MMU initialization:
//
// These structures are laid down in memory to define the translation
// table.
//

/*
 * ARM Domain Access Control Bit Masks
 */
#define ARC_ARM_ACCESS_TYPE_NO_ACCESS(domain_num)    (0x0 << (domain_num)*2)
#define ARC_ARM_ACCESS_TYPE_CLIENT(domain_num)       (0x1 << (domain_num)*2)
#define ARC_ARM_ACCESS_TYPE_MANAGER(domain_num)      (0x3 << (domain_num)*2)

#define ARC_ARM_FIRST_LEVEL_PAGE_TABLE_SIZE          0x4000
#define ARC_ARM_SECOND_LEVEL_PAGE_TABLE_SIZE         0x400
#define ARC_ARM_SECOND_LEVEL_FINE_TABLE_SIZE         0x1000
#define ARC_ARM_SECTION_SIZE                         0x100000
#define ARC_ARM_LARGE_PAGE_SIZE                      0x10000
#define ARC_ARM_SMALL_PAGE_SIZE                      0x1000
#define ARC_ARM_TINY_PAGE_SIZE                       0x400


#define ARC_ARM_MMU_FIRST_LEVEL_FAULT_ID             0x0
#define ARC_ARM_MMU_FIRST_LEVEL_PAGE_TABLE_ID        0x1
#define ARC_ARM_MMU_FIRST_LEVEL_SECTION_ID           0x2
#define ARC_ARM_MMU_FIRST_LEVEL_FINE_TABLE_ID        0x3

struct ARC_ARM_MMU_FIRST_LEVEL_FAULT {
    uint32_t id : 2;
    uint32_t sbz : 30;
};

struct ARC_ARM_MMU_FIRST_LEVEL_PAGE_TABLE {
    uint32_t id : 2;
    uint32_t sbz0 : 2;
    uint32_t imp : 1;
    uint32_t domain : 4;
    uint32_t sbz1 : 1;
    uint32_t base_address : 22;
};

struct ARC_ARM_MMU_FIRST_LEVEL_SECTION {
    uint32_t id : 2;
    uint32_t b : 1;
    uint32_t c : 1;
    uint32_t imp : 1;
    uint32_t domain : 4;
    uint32_t sbz0 : 1;
    uint32_t ap : 2;
    uint32_t sbz1 : 8;
    uint32_t base_address : 12;
};

struct ARC_ARM_MMU_FIRST_LEVEL_FINE_TABLE {
    uint32_t id : 2;
    uint32_t sbz0 : 2;
    uint32_t imp : 1;
    uint32_t domain : 4;
    uint32_t sbz : 3;
    uint32_t base_address : 20;
};

union ARC_ARM_MMU_FIRST_LEVEL_DESCRIPTOR {
    uint32_t word;
    struct ARC_ARM_MMU_FIRST_LEVEL_FAULT fault;
    struct ARC_ARM_MMU_FIRST_LEVEL_PAGE_TABLE page_table;
    struct ARC_ARM_MMU_FIRST_LEVEL_SECTION section;
    struct ARC_ARM_MMU_FIRST_LEVEL_FINE_TABLE fine_table;
};

#define ARC_ARM_MMU_FIRST_LEVEL_DESCRIPTOR_ADDRESS(ttb_base, table_index) \
   (uint32_t *)((uint32_t)(ttb_base) + ((table_index) << 2))


#define ARC_ARM_MMU_SECOND_LEVEL_FAULT_ID            0x0
#define ARC_ARM_MMU_SECOND_LEVEL_LARGE_PAGE_ID       0x1
#define ARC_ARM_MMU_SECOND_LEVEL_SMALL_PAGE_ID       0x2
#define ARC_ARM_MMU_SECOND_LEVEL_TINY_PAGE_ID        0x3

struct ARC_ARM_MMU_SECOND_LEVEL_FAULT {
    uint32_t id : 2;
    uint32_t sbz : 30;
};

struct ARC_ARM_MMU_SECOND_LEVEL_LARGE_PAGE {
    uint32_t id : 2;
    uint32_t b : 1;
    uint32_t c : 1;
    uint32_t ap0 : 2;
    uint32_t ap1 : 2;
    uint32_t ap2 : 2;
    uint32_t ap3 : 2;
    uint32_t sbz : 4;
    uint32_t base_address : 16;
};

struct ARC_ARM_MMU_SECOND_LEVEL_SMALL_PAGE {
    uint32_t id : 2;
    uint32_t b : 1;
    uint32_t c : 1;
    uint32_t ap0 : 2;
    uint32_t ap1 : 2;
    uint32_t ap2 : 2;
    uint32_t ap3 : 2;
    uint32_t base_address : 20;
};

struct ARC_ARM_MMU_SECOND_LEVEL_TINY_PAGE {
    uint32_t id : 2;
    uint32_t b : 1;
    uint32_t c : 1;
    uint32_t ap : 2;
    uint32_t sbz : 4;
    uint32_t base_address : 22;
};

union ARC_ARM_MMU_SECOND_LEVEL_DESCRIPTOR {
    uint32_t word;
    struct ARC_ARM_MMU_SECOND_LEVEL_FAULT fault;
    struct ARC_ARM_MMU_SECOND_LEVEL_LARGE_PAGE large_page;
    struct ARC_ARM_MMU_SECOND_LEVEL_SMALL_PAGE small_page;
    struct ARC_ARM_MMU_SECOND_LEVEL_TINY_PAGE tiny_page;
};

#define ARC_ARM_MMU_SECOND_LEVEL_DESCRIPTOR_ADDRESS(page_table, table_index) \
   (uint32_t *)((uint32_t)(page_table) + ((table_index) << 2))



#define ARC_ARM_MMU_SECTION(ttb_base, actual_base, virtual_base,            \
                            cacheable, bufferable, perm)                    \
    do { 			                                                         \
    register union ARC_ARM_MMU_FIRST_LEVEL_DESCRIPTOR desc;                 \
    desc.word = 0;                                                          \
    desc.section.id = ARC_ARM_MMU_FIRST_LEVEL_SECTION_ID;                   \
    desc.section.imp = 1;                                                   \
    desc.section.domain = 0;                                                \
    desc.section.c = (cacheable);                                           \
    desc.section.b = (bufferable);                                          \
    desc.section.ap = (perm);                                               \
    desc.section.base_address = (actual_base);							    \
    *ARC_ARM_MMU_FIRST_LEVEL_DESCRIPTOR_ADDRESS((ttb_base), (virtual_base)) \
                            = desc.word;                                    \
    } while (0)

#define ARC_ARM_MMU_PAGE_TABLE(ttb_base, actual_base, virtual_base)         \
    do {  			                                                       \
    register union ARC_ARM_MMU_FIRST_LEVEL_DESCRIPTOR desc;                 \
    desc.word = 0;                                                          \
    desc.page_table.id = ARC_ARM_MMU_FIRST_LEVEL_PAGE_TABLE_ID;             \
    desc.page_table.imp = 1;                                                \
    desc.page_table.domain = 0;                                             \
    desc.page_table.base_address = (actual_base);                           \
    *ARC_ARM_MMU_FIRST_LEVEL_DESCRIPTOR_ADDRESS((ttb_base), (virtual_base)) \
                            = desc.word;                                    \
    } while (0)

#define ARC_ARM_MMU_FINE_TABLE(ttb_base, actual_base, virtual_base)         \
    do {			                                                         \
    register union ARC_ARM_MMU_FIRST_LEVEL_DESCRIPTOR desc;                 \
    desc.word = 0;                                                          \
    desc.fine_table.id = ARC_ARM_MMU_FIRST_LEVEL_FINE_TABLE_ID;             \
    desc.fine_table.imp = 1;                                                \
    desc.fine_table.domain = 0;                                             \
    desc.fine_table.base_address = (actual_base);                           \
    *ARC_ARM_MMU_FIRST_LEVEL_DESCRIPTOR_ADDRESS((ttb_base), (virtual_base)) \
                            = desc.word;                                    \
    } while (0)

#define ARC_ARM_MMU_LARGE_PAGE(page_table, actual_base, virtual_base,       \
                               cacheable, bufferable, perm0, perm1, perm2, perm3) \
    do {			                                                         \
    register union ARC_ARM_MMU_SECOND_LEVEL_DESCRIPTOR desc;                \
    desc.word = 0;                                                          \
    desc.large_page.id = ARC_ARM_MMU_SECOND_LEVEL_LARGE_PAGE_ID;            \
    desc.large_page.c = (cacheable);                                        \
    desc.large_page.b = (bufferable);                                       \
    desc.large_page.ap0 = (perm0);                                          \
    desc.large_page.ap1 = (perm1);                                          \
    desc.large_page.ap2 = (perm2);                                          \
    desc.large_page.ap3 = (perm3);                                          \
    desc.large_page.base_address = (actual_base);                           \
    *ARC_ARM_MMU_SECOND_LEVEL_DESCRIPTOR_ADDRESS((page_table), (virtual_base)) \
                            = desc.word;                                    \
    } while (0)

#define ARC_ARM_MMU_SMALL_PAGE(page_table, actual_base, virtual_base,       \
                               cacheable, bufferable, perm0, perm1, perm2, perm3) \
    do {   			                                                        \
    register union ARC_ARM_MMU_SECOND_LEVEL_DESCRIPTOR desc;                \
    desc.word = 0;                                                          \
    desc.small_page.id = ARC_ARM_MMU_SECOND_LEVEL_SMALL_PAGE_ID;            \
    desc.small_page.c = (cacheable);                                        \
    desc.small_page.b = (bufferable);                                       \
    desc.small_page.ap0 = (perm0);                                          \
    desc.small_page.ap1 = (perm1);                                          \
    desc.small_page.ap2 = (perm2);                                          \
    desc.small_page.ap3 = (perm3);                                          \
    desc.small_page.base_address = (actual_base);                           \
    *ARC_ARM_MMU_SECOND_LEVEL_DESCRIPTOR_ADDRESS((page_table), (virtual_base)) \
                            = desc.word;                                    \
    } while (0)

#define ARC_ARM_MMU_TINY_PAGE(page_table, actual_base, virtual_base,        \
                              cacheable, bufferable, perm)                  \
    do { 			                                                        \
    register union ARC_ARM_MMU_SECOND_LEVEL_DESCRIPTOR desc;                \
    desc.word = 0;                                                          \
    desc.tiny_page.id = ARC_ARM_MMU_SECOND_LEVEL_TINY_PAGE_ID;              \
    desc.tiny_page.c = (cacheable);                                         \
    desc.tiny_page.b = (bufferable);                                        \
    desc.tiny_page.ap = (perm);                                             \
    desc.tiny_page.base_address = (actual_base);                            \
    *ARC_ARM_MMU_SECOND_LEVEL_DESCRIPTOR_ADDRESS((page_table), (virtual_base)) \
                            = desc.word;                                    \
    } while (0)

/* The macro ARC_X_ARM_MMU_SECTION is used to define a memory map.
 *
 * abase, vbase, size : in units of MB
 * abase  : physical memory start address in MB
 * vbase  : virtual memory atart address in MB
 * size   : memory size in MB
 * cache  : if memory is cacheable or not
 * buff   : if memory is write back or write thru
 * access : access rights to be used
 *
 * note: the variable ttb_base is used implicitly
 * ttb_base is the physical address of the first level page table
 */
#define ARC_X_ARM_MMU_SECTION(abase,vbase,size,cache,buff,access)           \
    do {			                                                         \
    int numpages; int phys = (abase); int virt = (vbase);                   \
    for (numpages = (size); numpages > 0 ; numpages--, phys++, virt++)      \
    {                                                                       \
        ARC_ARM_MMU_SECTION(ttb_base, phys, virt,                           \
                            (cache), (buff), (access));                     \
    }                                                                       \
    } while (0)

/* The macro ARC_X_ARM_MMU_PAGE_TABLE is used to define a memory map.
 *
 * abase, vbase, vprotect, vend, atable : in units of byte
 * abase, vbase, vend must be a multiple of 4K
 * abase    : physical memory start address
 * vbase    : virtual memory start address
 * vprotect : virtual address of .data segment
 * vend     : virtual memory end address
 * atable   : physical address of second level page table
 * cache    : if memory is cacheable or not
 * buff     : if memory is write back or write thru
 * access0  : access rights for memory below vprotect
 * access1  : access rights for memory at or above vprotect
 *
 * vbase .. vprotect use access0
 * vprotect .. vend use access1
 * vprotect is rounded down to 1K regions
 *
 * atable is the physical address of a
 * 1K memory block used as coarse page table
 * atable must be a multiple of 1K
 *
 * note: the variable ttb_base is used implicitly
 * ttb_base is the physical address of the first level page table
 */
#define ARC_X_ARM_MMU_PAGE_TABLE(abase,vbase,vprotect,vend,atable,          \
                                 cache,buff,access0,access1)                \
    do {			                                                         \
    int numpages = (vend)-(vbase);                                          \
    int numprotect = (vprotect)-(vbase);                                    \
    int phys = (abase) >> 12; int virt = (vbase) >> 12;                     \
    numpages = numpages >> 12;                                              \
    numprotect = numprotect >> 10;                                          \
    while( numpages > 0 )                                                   \
    {                                                                       \
        if ( !(phys & 0xFF) && !(virt & 0xFF) && (numpages >= 0x100) &&     \
             !((numprotect > 0) && (numprotect < 0x400)) )                  \
        {                                                                   \
            ARC_ARM_MMU_SECTION(ttb_base, phys>>8, virt>>8, (cache), (buff),\
                                numprotect>0 ? (access0) : (access1));      \
            phys += 0x100; virt += 0x100; numpages -= 0x100;                \
            numprotect -= 0x400;                                            \
        }                                                                   \
        else                                                                \
        {                                                                   \
            ARC_ARM_MMU_PAGE_TABLE(ttb_base, (atable)>>10, virt>>8);        \
            if ( !(phys & 0xF) && !(virt & 0xF) && (numpages >= 0x10) &&    \
                 !((numprotect > 0) && (numprotect < 0x40) && (numprotect & 0xF)) ) \
            {                                                               \
                do                                                          \
                ARC_ARM_MMU_LARGE_PAGE((atable), phys>>4, virt++ & 0xFF,    \
                  (cache), (buff), numprotect>0x00 ? (access0) : (access1), \
                                   numprotect>0x10 ? (access0) : (access1), \
                                   numprotect>0x20 ? (access0) : (access1), \
                                   numprotect>0x30 ? (access0) : (access1));\
                while(virt & 0xF);                                          \
                phys += 0x10; numpages -= 0x10;                             \
                numprotect -= 0x40;                                         \
            }                                                               \
            else                                                            \
            {                                                               \
                ARC_ARM_MMU_SMALL_PAGE((atable), phys, virt++ & 0xFF,       \
                  (cache), (buff), numprotect>0x0 ? (access0) : (access1),  \
                                   numprotect>0x1 ? (access0) : (access1),  \
                                   numprotect>0x2 ? (access0) : (access1),  \
                                   numprotect>0x3 ? (access0) : (access1)); \
                phys ++; numpages --; numprotect -= 4;                      \
            }                                                               \
        }                                                                   \
    }                                                                       \
    } while (0)

/* The macro ARC_X_ARM_MMU_FINE_TABLE is used to define a memory map.
 *
 * abase, vbase, vend, atable : in units of byte
 * abase, vbase, vend must be a multiple of 1K
 * abase  : physical memory start address
 * vbase  : virtual memory start address
 * vend   : virtual memory end address
 * atable : physical address of second level page table
 * cache  : if memory is cacheable or not
 * buff   : if memory is write back or write thru
 * access : access rights to be used
 *
 * atable is the physical address of a
 * 4K memory block used as fine page table
 * atable must be a multiple of 4K
 *
 * note: the variable ttb_base is used implicitly
 * ttb_base is the physical address of the first level page table
 */
#define ARC_X_ARM_MMU_FINE_TABLE(abase,vbase,vend,atable,cache,buff,access) \
    do {			                                                         \
    int numpages = (vend)-(vbase);                                          \
    int phys = (abase) >> 10; int virt = (vbase) >> 10;                     \
    numpages = numpages >> 10;                                              \
    while( numpages > 0 )                                                   \
    {                                                                       \
        if ( !(phys & 0x3FF) && !(virt & 0x3FF) && (numpages >= 0x400) )    \
        {                                                                   \
            ARC_ARM_MMU_SECTION(ttb_base, phys>>10, virt>>10,               \
                                (cache), (buff), (access));                 \
            phys += 0x400; virt += 0x400; numpages -= 0x400;                \
        }                                                                   \
        else                                                                \
        {                                                                   \
            ARC_ARM_MMU_FINE_TABLE(ttb_base, (atable)>>12, virt>>10);       \
            if ( !(phys & 0x3F) && !(virt & 0x3F) && (numpages >= 0x40) )   \
            {                                                               \
                do                                                          \
                ARC_ARM_MMU_LARGE_PAGE((atable), phys>>6, virt++ & 0x3FF,   \
                  (cache), (buff), (access), (access), (access), (access)); \
                while(virt & 0x3F);                                         \
                phys += 0x40; numpages -= 0x40;                             \
            }                                                               \
            else if ( !(phys & 0x3) && !(virt & 0x3) && (numpages >= 0x4) ) \
            {                                                               \
                do                                                          \
                ARC_ARM_MMU_SMALL_PAGE((atable), phys>>2, virt++ & 0x3FF,   \
                  (cache), (buff), (access), (access), (access), (access)); \
                while(virt & 0x3);                                          \
                phys += 0x4; numpages -= 0x4;                               \
            }                                                               \
            else                                                            \
            {                                                               \
                ARC_ARM_MMU_TINY_PAGE((atable), phys, virt++ & 0x3FF,       \
                                      (cache), (buff), (access));           \
                phys ++; numpages --;                                       \
            }                                                               \
        }                                                                   \
    }                                                                       \
    } while (0)

#define ARC_ARM_UNCACHEABLE                         0
#define ARC_ARM_CACHEABLE                           1
#define ARC_ARM_UNBUFFERABLE                        0
#define ARC_ARM_BUFFERABLE                          1

#define ARC_ARM_ACCESS_PERM_NONE_NONE               0
#define ARC_ARM_ACCESS_PERM_RO_NONE                 0
#define ARC_ARM_ACCESS_PERM_RO_RO                   0
#define ARC_ARM_ACCESS_PERM_RW_NONE                 1
#define ARC_ARM_ACCESS_PERM_RW_RO                   2
#define ARC_ARM_ACCESS_PERM_RW_RW                   3



/** \brief  Enable MMU
*/
__STATIC_INLINE void MMU_Enable(void)
{
  // Set M bit 0 to enable the MMU
  // Set AFE bit to enable simplified access permissions model
  // Clear TRE bit to disable TEX remap and A bit to disable strict alignment fault checking
  __set_SCTLR( (__get_SCTLR() & ~(1 << 28) & ~(1 << 1)) | 1 | (1 << 29));
}

/** \brief  Disable MMU
*/
__STATIC_INLINE void MMU_Disable(void)
{
  // Clear M bit 0 to disable the MMU
  __set_SCTLR( __get_SCTLR() & ~1);
}

/** \brief  Invalidate entire unified TLB
*/

__STATIC_INLINE void MMU_InvalidateTLB(void)
{
  __set_TLBIALL(0);
}


/* ##########################  MMU functions  ###################################### */
#define SECTION_DESCRIPTOR      (0x2)
#define SECTION_MASK            (0xFFFFFFFC)

#define SECTION_TEXCB_MASK      (0xFFFF8FF3)
#define SECTION_B_SHIFT         (2)
#define SECTION_C_SHIFT         (3)
#define SECTION_TEX0_SHIFT      (12)			//ARM926 must be 0
#define SECTION_TEX1_SHIFT      (13)			//ARM926 must be 0
#define SECTION_TEX2_SHIFT      (14)			//ARM926 must be 0

#define SECTION_XN_MASK         (0xFFFFFFEF)	//ARM926 not use, must be 1 , 1 not excute domain, 0 excutable domain
#define SECTION_XN_SHIFT        (4)

#define SECTION_DOMAIN_MASK     (0xFFFFFE1F)
#define SECTION_DOMAIN_SHIFT    (5)	

#define SECTION_P_MASK          (0xFFFFFDFF)	//ARM926 must be 0, ARM926 not have ECC fucntion
#define SECTION_P_SHIFT         (9)

//#define SECTION_AP_MASK         (0xFFFF73FF)
#define SECTION_AP_MASK         (0xFFFFF3FF)
#define SECTION_AP_SHIFT        (10)
//#define SECTION_AP2_SHIFT       (15)			//ARM926 not have the bit ,it must be 0

#define SECTION_S_MASK          (0xFFFEFFFF)
#define SECTION_S_SHIFT         (16)			//ARM926 not have the bit ,it must be 0

#define SECTION_NG_MASK         (0xFFFDFFFF)
#define SECTION_NG_SHIFT        (17)			//ARM926 not have the bit ,it must be 0

#define SECTION_NS_MASK         (0xFFF7FFFF)
#define SECTION_NS_SHIFT        (19)			//ARM926 not have the bit ,it must be 0,no secure

#define PAGE_L1_DESCRIPTOR      (0x1)
#define PAGE_L1_MASK            (0xFFFFFFFC)

#define PAGE_L2_4K_DESC         (0x2)
#define PAGE_L2_4K_MASK         (0xFFFFFFFD)

#define PAGE_L2_64K_DESC        (0x1)
#define PAGE_L2_64K_MASK        (0xFFFFFFFC)

#define PAGE_4K_TEXCB_MASK      (0xFFFFF003)
#define PAGE_4K_B_SHIFT         (2)
#define PAGE_4K_C_SHIFT         (3)
#define PAGE_4K_AP0_SHIFT		(4)
#define PAGE_4K_AP1_SHIFT		(6)
#define PAGE_4K_AP2_SHIFT		(8)
#define PAGE_4K_AP3_SHIFT		(10)
#define PAGE_4K_TEX0_SHIFT      (6)			//ARM926 not have the bit
#define PAGE_4K_TEX1_SHIFT      (7)			//ARM926 not have the bit
#define PAGE_4K_TEX2_SHIFT      (8)			//ARM926 not have the bit

#define PAGE_64K_TEXCB_MASK     (0xFFFFF003)
#define PAGE_64K_B_SHIFT        (2)
#define PAGE_64K_C_SHIFT        (3)
#define PAGE_64K_AP0_SHIFT		(4)
#define PAGE_64K_AP1_SHIFT		(6)
#define PAGE_64K_AP2_SHIFT		(8)
#define PAGE_64K_AP3_SHIFT		(10)
#define PAGE_64K_TEX0_SHIFT     (12)
#define PAGE_64K_TEX1_SHIFT     (13)
#define PAGE_64K_TEX2_SHIFT     (14)

#define PAGE_TEXCB_MASK         (0xFFFF8FF3)
#define PAGE_B_SHIFT            (2)
#define PAGE_C_SHIFT            (3)
#define PAGE_TEX_SHIFT          (12)			//ARM926 not have the bit 

#define PAGE_XN_4K_MASK         (0xFFFFFFFE)	
#define PAGE_XN_4K_SHIFT        (0)				//ARM926 not have the bit 
#define PAGE_XN_64K_MASK        (0xFFFF7FFF)
#define PAGE_XN_64K_SHIFT       (15)			//ARM926 not have the bit 	

#define PAGE_DOMAIN_MASK        (0xFFFFFE1F)	 
#define PAGE_DOMAIN_SHIFT       (5)				//ARM926 not have the bit

#define PAGE_P_MASK             (0xFFFFFDFF)	
#define PAGE_P_SHIFT            (9)				//ARM926 not have the bit

#define PAGE_AP_MASK            (0xFFFFF00F)
#define PAGE_AP_SHIFT           (4)	
#define PAGE_AP1_SHIFT          (6)	
#define PAGE_AP2_SHIFT          (8)	
#define PAGE_AP3_SHIFT          (10)




#define PAGE_S_MASK             (0xFFFFFBFF)
#define PAGE_S_SHIFT            (10)			//ARM926 not have the bit

#define PAGE_NG_MASK            (0xFFFFF7FF)
#define PAGE_NG_SHIFT           (11)			//ARM926 not have the bit

#define PAGE_NS_MASK            (0xFFFFFFF7)
#define PAGE_NS_SHIFT           (3)				//ARM926 not have the bit

#define OFFSET_1M               (0x00100000)
#define OFFSET_64K              (0x00010000)
#define OFFSET_4K               (0x00001000)

#define DESCRIPTOR_FAULT        (0x00000000)

/* Attributes enumerations */

/* Region size attributes */
typedef enum
{
   SECTION,
   PAGE_4k,
   PAGE_64k,
} mmu_region_size_Type;

/* Region type attributes */
typedef enum
{
   NORMAL,
   DEVICE,
   SHARED_DEVICE,
   NON_SHARED_DEVICE,
   STRONGLY_ORDERED
} mmu_memory_Type;

/* Region cacheability attributes */
typedef enum
{
   NON_CACHEABLE,
   WB_WA,
   WT,
   WB_NO_WA,
} mmu_cacheability_Type;


/* Region parity check attributes */
typedef enum
{
   ECC_DISABLED,
   ECC_ENABLED,
} mmu_ecc_check_Type;
   

/* Region execution attributes */
typedef enum
{
   EXECUTE,
   NON_EXECUTE,
} mmu_execute_Type;

/* Region global attributes */
typedef enum
{
   GLOBAL,
   NON_GLOBAL,
} mmu_global_Type;

/* Region shareability attributes */
typedef enum
{
   NON_SHARED,
   SHARED,
} mmu_shared_Type;

/* Region security attributes */
typedef enum
{
   SECURE,
   NON_SECURE,
} mmu_secure_Type;

/* Region access attributes */
typedef enum
{
   NO_ACCESS,
   RW,
   READ,
} mmu_access_Type;


/* Memory Region definition */
typedef struct RegionStruct {
    mmu_region_size_Type rg_t;
    mmu_memory_Type mem_t;
    uint8_t domain;
    mmu_cacheability_Type inner_norm_t;
//  mmu_cacheability_Type outer_norm_t;
//  mmu_ecc_check_Type e_t;
//  mmu_execute_Type xn_t;
//  mmu_global_Type g_t;
//  mmu_secure_Type sec_t;
    mmu_access_Type priv_t;
    mmu_access_Type user_t;
//  mmu_shared_Type sh_t;

} mmu_region_attributes_Type;


//Following macros define the descriptors and attributes
//Sect_Normal. Outer & inner wb/wa, non-shareable, executable, rw, domain 0
#define section_normal(descriptor_l1, region)     region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.inner_norm_t = WB_NO_WA; \
                                   region.mem_t = NORMAL; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   MMU_GetSectionDescriptor(&descriptor_l1, region); 

//Sect_Normal_NC. Outer & inner non-cacheable, non-shareable, executable, rw, domain 0
#define section_normal_nc(descriptor_l1, region)     region.rg_t = SECTION; \
								  region.domain = 0x0; \
								  region.inner_norm_t = NON_CACHEABLE; \
								  region.mem_t = NORMAL; \
								  region.priv_t = RW; \
								  region.user_t = RW; \
								  MMU_GetSectionDescriptor(&descriptor_l1, region);


//Sect_Normal_RW. Sect_Normal_Cod, but writeable and not executable
#define section_normal_rw(descriptor_l1, region) region.rg_t = SECTION; \
							   region.domain = 0x0; \
							   region.inner_norm_t = WB_WA; \
							   region.mem_t = NORMAL; \
							   region.priv_t = RW; \
							   region.user_t = RW; \
							   MMU_GetSectionDescriptor(&descriptor_l1, region);


//Sect_SO. Strongly-ordered (therefore shareable), not executable, rw, domain 0, base addr 0
#define section_so(descriptor_l1, region) region.rg_t = SECTION; \
							  region.domain = 0x0; \
							  region.inner_norm_t = NON_CACHEABLE; \
							  region.mem_t = STRONGLY_ORDERED; \
							  region.priv_t = RW; \
							  region.user_t = RW; \
							  MMU_GetSectionDescriptor(&descriptor_l1, region);


//Sect_Device_RW. Sect_Device_RO, but writeable
#define section_device_rw(descriptor_l1, region) region.rg_t = SECTION; \
							region.domain = 0x0; \
							region.inner_norm_t = NON_CACHEABLE; \
							region.mem_t = STRONGLY_ORDERED; \
							region.priv_t = RW; \
							region.user_t = RW; \
							MMU_GetSectionDescriptor(&descriptor_l1, region);

//Page_4k_Device_RW.  Shared device, not executable, rw, domain 0
#define page4k_device_rw(descriptor_l1, descriptor_l2, region) region.rg_t = PAGE_4k; \
						   region.domain = 0x0; \
						   region.inner_norm_t = NON_CACHEABLE; \
						   region.priv_t = RW; \
						   region.user_t = RW; \
						   MMU_GetPageDescriptor(&descriptor_l1, &descriptor_l2, region);

//Page_64k_Device_RW.  Shared device, not executable, rw, domain 0
#define page64k_device_rw(descriptor_l1, descriptor_l2, region)  region.rg_t = PAGE_64k; \
						  region.domain = 0x0; \
						  region.inner_norm_t = NON_CACHEABLE; \
						  region.priv_t = RW; \
						  region.user_t = RW; \
						  MMU_GetPageDescriptor(&descriptor_l1, &descriptor_l2, region);



/** \brief  Set section domain

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]            domain  Section domain

  \return          0
*/
__STATIC_INLINE int MMU_DomainSection(uint32_t *descriptor_l1, uint8_t domain)
{
  *descriptor_l1 &= SECTION_DOMAIN_MASK;
  *descriptor_l1 |= ((domain & 0xF) << SECTION_DOMAIN_SHIFT);
  return 0;
}


/** \brief  Set section access privileges

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]              user  User Level Access: NO_ACCESS, RW, READ
  \param [in]              priv  Privilege Level Access: NO_ACCESS, RW, READ
  \param [in]               afe  Access flag enable

  \return          0
*/
__STATIC_INLINE int MMU_APSection(uint32_t *descriptor_l1, mmu_access_Type user, mmu_access_Type priv, uint32_t afe)
{
  uint32_t ap = 0;

  if (afe == 0) { //full access
    if ((priv == NO_ACCESS) && (user == NO_ACCESS)) { ap = 0x0; }
    else if ((priv == RW) && (user == NO_ACCESS))   { ap = 0x1; }
    else if ((priv == RW) && (user == READ))        { ap = 0x2; }
    else if ((priv == RW) && (user == RW))          { ap = 0x3; }
  }

  else { //Simplified access
    if ((priv == RW) && (user == NO_ACCESS))        { ap = 0x1; }
	else if ((priv == RW) && (user == READ))        { ap = 0x2; }
    else if ((priv == RW) && (user == RW))          { ap = 0x3; }
  }

  *descriptor_l1 &= SECTION_AP_MASK;
  *descriptor_l1 |= (ap & 0x3) << SECTION_AP_SHIFT;

  return 0;
}


/** \brief  Set 4k/64k page domain

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]            domain  Page domain

  \return          0
*/
__STATIC_INLINE int MMU_DomainPage(uint32_t *descriptor_l1, uint8_t domain)
{
  *descriptor_l1 &= PAGE_DOMAIN_MASK;
  *descriptor_l1 |= ((domain & 0xf) << PAGE_DOMAIN_SHIFT);
  return 0;
}


/** \brief  Set 4k/64k page access privileges

  \param [out]    descriptor_l2  L2 descriptor.
  \param [in]              user  User Level Access: NO_ACCESS, RW, READ
  \param [in]              priv  Privilege Level Access: NO_ACCESS, RW, READ
  \param [in]               afe  Access flag enable

  \return          0
*/
__STATIC_INLINE int MMU_APPage(uint32_t *descriptor_l2, mmu_access_Type user, mmu_access_Type priv, uint32_t afe)
{
  uint32_t ap = 0;

  if (afe == 0) { //full access
    if ((priv == NO_ACCESS) && (user == NO_ACCESS)) { ap = 0x0; }
    else if ((priv == RW) && (user == NO_ACCESS))   { ap = 0x1; }
    else if ((priv == RW) && (user == READ))        { ap = 0x2; }
    else if ((priv == RW) && (user == RW))          { ap = 0x3; }
  }

  else { //Simplified access
    if ((priv == RW) && (user == NO_ACCESS))        { ap = 0x1; }
    else if ((priv == RW) && (user == RW))          { ap = 0x3; }
  }

  *descriptor_l2 &= PAGE_AP_MASK;
  *descriptor_l2 |= (ap & 0x3) << PAGE_AP_SHIFT;
  *descriptor_l2 |= (ap & 0x3) << PAGE_AP1_SHIFT;
  *descriptor_l2 |= (ap & 0x3) << PAGE_AP2_SHIFT;
  *descriptor_l2 |= (ap & 0x3) << PAGE_AP3_SHIFT;

  return 0;
}


/** \brief  Set Section memory attributes

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]               mem  Section memory type: NORMAL, DEVICE, SHARED_DEVICE, NON_SHARED_DEVICE, STRONGLY_ORDERED
  \param [in]             inner  Inner cacheability: NON_CACHEABLE, WB_WA, WT, WB_NO_WA,

  \return          0
*/

__STATIC_INLINE int MMU_MemorySection(uint32_t *descriptor_l1, 
 	mmu_memory_Type mem, 
 	mmu_cacheability_Type inner)
{
  *descriptor_l1 &= SECTION_TEXCB_MASK;

  if (STRONGLY_ORDERED == mem)
  {
    return 0;
  }
  else if (SHARED_DEVICE == mem)
  {
    *descriptor_l1 |= (1 << SECTION_B_SHIFT);
  }
  else if (NORMAL == mem)
  {
 // *descriptor_l1 |= 1 << SECTION_TEX2_SHIFT;
   switch(inner)
   {
      case NON_CACHEABLE:
        break;
      case WB_WA:
        *descriptor_l1 |= (1 << SECTION_B_SHIFT);
        break;
      case WT:
        *descriptor_l1 |= 1 << SECTION_C_SHIFT;
        break;
      case WB_NO_WA:
        *descriptor_l1 |= (1 << SECTION_B_SHIFT) | (1 << SECTION_C_SHIFT);
        break;
    }
  }
  return 0;
}

/** \brief  Set 4k/64k page memory attributes

  \param [out]    descriptor_l2  L2 descriptor.
  \param [in]               mem  4k/64k page memory type: NORMAL, DEVICE, SHARED_DEVICE, NON_SHARED_DEVICE, STRONGLY_ORDERED
  \param [in]             outer  Outer cacheability: NON_CACHEABLE, WB_WA, WT, WB_NO_WA,
  \param [in]             inner  Inner cacheability: NON_CACHEABLE, WB_WA, WT, WB_NO_WA,
  \param [in]              page  Page size

  \return          0
*/
//__STATIC_INLINE int MMU_MemoryPage(uint32_t *descriptor_l2, mmu_memory_Type mem, mmu_cacheability_Type outer, mmu_cacheability_Type inner, mmu_region_size_Type page)
__STATIC_INLINE int MMU_MemoryPage(uint32_t *descriptor_l2, 
 mmu_memory_Type mem, 
 mmu_cacheability_Type inner, 
 mmu_region_size_Type page)

{
  *descriptor_l2 &= PAGE_4K_TEXCB_MASK;

  if (page == PAGE_64k)
  {
    //same as section
    MMU_MemorySection(descriptor_l2, mem, inner);
  }
  else
  {
    if (STRONGLY_ORDERED == mem)
    {
      return 0;
    }
    else if (SHARED_DEVICE == mem)
    {
      *descriptor_l2 |= (1 << PAGE_4K_B_SHIFT);
    }
    else if (NORMAL == mem)
    {
     // *descriptor_l2 |= 1 << PAGE_4K_TEX2_SHIFT;
      switch(inner)
      {
        case NON_CACHEABLE:
          break;
        case WB_WA:
          *descriptor_l2 |= (1 << PAGE_4K_B_SHIFT);
          break;
        case WT:
          *descriptor_l2 |= 1 << PAGE_4K_C_SHIFT;
          break;
        case WB_NO_WA:
          *descriptor_l2 |= (1 << PAGE_4K_B_SHIFT) | (1 << PAGE_4K_C_SHIFT);
          break;
      }
    }

  }

  return 0;
}


 

/** \brief  Create a L1 section descriptor

  \param [out]     descriptor  L1 descriptor
  \param [in]      reg  Section attributes
  
  \return          0
*/


__STATIC_INLINE int MMU_GetSectionDescriptor(uint32_t *descriptor, mmu_region_attributes_Type reg)
{

  *descriptor  = 0;
  MMU_MemorySection(descriptor, reg.mem_t, reg.inner_norm_t);
  MMU_DomainSection(descriptor, reg.domain);
  MMU_APSection(descriptor, reg.priv_t, reg.user_t, 1);

  *descriptor |= 1<<SECTION_XN_SHIFT; 
  *descriptor &= SECTION_MASK;
  *descriptor |= SECTION_DESCRIPTOR;
  return 0;
}

/** \brief  Create a L1 and L2 4k/64k page descriptor

  \param [out]       descriptor  L1 descriptor
  \param [out]      descriptor2  L2 descriptor
  \param [in]               reg  4k/64k page attributes

  \return          0
*/
__STATIC_INLINE int MMU_GetPageDescriptor(uint32_t *descriptor, 
 	uint32_t *descriptor2, 
 	mmu_region_attributes_Type reg)
{
  *descriptor  = 0;
  *descriptor2 = 0;

  switch (reg.rg_t)
  {
    case PAGE_4k:
      MMU_MemoryPage(descriptor2, reg.mem_t, reg.inner_norm_t, PAGE_4k);
      MMU_DomainPage(descriptor, reg.domain);
      MMU_APPage(descriptor2, reg.priv_t, reg.user_t, 1);
      *descriptor &= PAGE_L1_MASK;
      *descriptor |= PAGE_L1_DESCRIPTOR;
      *descriptor2 &= PAGE_L2_4K_MASK;
      *descriptor2 |= PAGE_L2_4K_DESC;
      break;

    case PAGE_64k:
      MMU_MemoryPage(descriptor2, reg.mem_t, reg.inner_norm_t, PAGE_64k);
      MMU_DomainPage(descriptor, reg.domain);
      MMU_APPage(descriptor2, reg.priv_t, reg.user_t, 1);
      *descriptor &= PAGE_L1_MASK;
      *descriptor |= PAGE_L1_DESCRIPTOR;
      *descriptor2 &= PAGE_L2_64K_MASK;
      *descriptor2 |= PAGE_L2_64K_DESC;
      break;

    case SECTION:
      //error
      break;
  }
  
  return 0;
}

/** \brief  Create a 1MB Section

  \param [in]               ttb  Translation table base address
  \param [in]      base_address  Section base address
  \param [in]             count  Number of sections to create
  \param [in]     descriptor_l1  L1 descriptor (region attributes)

*/
__STATIC_INLINE void MMU_TTSection(uint32_t *ttb, 
 uint32_t base_address, 
 uint32_t count, 
 uint32_t descriptor_l1)
{
  uint32_t offset;
  uint32_t entry;
  uint32_t i;

  offset = base_address >> 20;
  entry  = (base_address & 0xFFF00000) | descriptor_l1;

  //4 bytes aligned
  ttb = ttb + offset;

  for (i = 0; i < count; i++ )
  {
    //4 bytes aligned
    *ttb++ = entry;
    entry += OFFSET_1M;
  }
}

/** \brief  Create a 4k page entry

  \param [in]               ttb  L1 table base address
  \param [in]      base_address  4k base address
  \param [in]             count  Number of 4k pages to create
  \param [in]     descriptor_l1  L1 descriptor (region attributes)
  \param [in]            ttb_l2  L2 table base address
  \param [in]     descriptor_l2  L2 descriptor (region attributes)

*/
__STATIC_INLINE void MMU_TTPage4k(uint32_t *ttb, 
 uint32_t base_address, 
 uint32_t count, 
 uint32_t descriptor_l1, 
 uint32_t *ttb_l2, 
 uint32_t descriptor_l2 )
{

  uint32_t offset, offset2;
  uint32_t entry, entry2;
  uint32_t i;

  offset = base_address >> 20;
  entry  = ((int)ttb_l2 & 0xFFFFFC00) | descriptor_l1;

  //4 bytes aligned
  ttb += offset;
  //create l1_entry
  *ttb = entry;

  offset2 = (base_address & 0xff000) >> 12;
  ttb_l2 += offset2;
  entry2 = (base_address & 0xFFFFF000) | descriptor_l2;
  for (i = 0; i < count; i++ )
  {
    //4 bytes aligned
    *ttb_l2++ = entry2;
    entry2 += OFFSET_4K;
  }
}

/** \brief  Create a 64k page entry

  \param [in]               ttb  L1 table base address
  \param [in]      base_address  64k base address
  \param [in]             count  Number of 64k pages to create
  \param [in]     descriptor_l1  L1 descriptor (region attributes)
  \param [in]            ttb_l2  L2 table base address
  \param [in]     descriptor_l2  L2 descriptor (region attributes)

*/
__STATIC_INLINE void MMU_TTPage64k(uint32_t *ttb, 
 uint32_t base_address, 
 uint32_t count, 
 uint32_t descriptor_l1, 
 uint32_t *ttb_l2, 
 uint32_t descriptor_l2 )
{
  uint32_t offset, offset2;
  uint32_t entry, entry2;
  uint32_t i,j;


  offset = base_address >> 20;
  entry  = ((int)ttb_l2 & 0xFFFFFC00) | descriptor_l1;

  //4 bytes aligned
  ttb += offset;
  //create l1_entry
  *ttb = entry;

  offset2 = (base_address & 0xff000) >> 12;
  ttb_l2 += offset2;
  entry2 = (base_address & 0xFFFF0000) | descriptor_l2;
  for (i = 0; i < count; i++ )
  {
    //create 16 entries
    for (j = 0; j < 16; j++)
    {
      //4 bytes aligned
      *ttb_l2++ = entry2;
    }
    entry2 += OFFSET_64K;
  }
}


#ifdef __cplusplus
}
#endif

#endif /* __CORE_CA_H_DEPENDANT */

#endif /* __CMSIS_GENERIC */

