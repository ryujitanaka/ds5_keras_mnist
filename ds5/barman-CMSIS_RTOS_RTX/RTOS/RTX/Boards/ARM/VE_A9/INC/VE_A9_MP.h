/**************************************************************************//**
 * @file     VE_A9_MP.h
 * @brief    CMSIS Cortex-A9 Core Peripheral Access Layer Header File 
 *           for the VE_A9_MP
 * @version  V1.00
 * @data     30 Oct 2015
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2011 - 2015 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#ifndef __VE_A9_MP_H__ 
#define __VE_A9_MP_H__

#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/******  SGI Interrupts Numbers                 ****************************************/
  SGI0_IRQn            =  0,
  SGI1_IRQn            =  1,
  SGI2_IRQn            =  2,
  SGI3_IRQn            =  3,
  SGI4_IRQn            =  4,
  SGI5_IRQn            =  5,
  SGI6_IRQn            =  6,
  SGI7_IRQn            =  7,
  SGI8_IRQn            =  8,
  SGI9_IRQn            =  9,
  SGI10_IRQn           = 10,
  SGI11_IRQn           = 11,
  SGI12_IRQn           = 12,
  SGI13_IRQn           = 13,
  SGI14_IRQn           = 14,
  SGI15_IRQn           = 15,

/******  Cortex-A9 Processor Exceptions Numbers ****************************************/
  GlobalTimer_IRQn     = 27,        /*!< Global Timer Interrupt                        */
  PrivTimer_IRQn       = 29,        /*!< Private Timer Interrupt                       */
  PrivWatchdog_IRQn    = 30,        /*!< Private Watchdog Interrupt                    */

/******  Platform Exceptions Numbers ***************************************************/
  Watchdog_IRQn        = 32,        /*!< SP805 Interrupt        */
  Timer0_IRQn          = 34,        /*!< SP804 Interrupt        */
  Timer1_IRQn          = 35,        /*!< SP804 Interrupt        */
  RTClock_IRQn         = 36,        /*!< PL031 Interrupt        */
  UART0_IRQn           = 37,        /*!< PL011 Interrupt        */
  UART1_IRQn           = 38,        /*!< PL011 Interrupt        */
  UART2_IRQn           = 39,        /*!< PL011 Interrupt        */
  UART3_IRQn           = 40,        /*!< PL011 Interrupt        */
  MCI0_IRQn            = 41,        /*!< PL180 Interrupt (1st)  */
  MCI1_IRQn            = 42,        /*!< PL180 Interrupt (2nd)  */
  AACI_IRQn            = 43,        /*!< PL041 Interrupt        */
  Keyboard_IRQn        = 44,        /*!< PL050 Interrupt        */
  Mouse_IRQn           = 45,        /*!< PL050 Interrupt        */
  CLCD_IRQn            = 46,        /*!< PL111 Interrupt        */
  Ethernet_IRQn        = 47,        /*!< SMSC_91C111 Interrupt  */
  VFS2_IRQn            = 73,        /*!< VFS2 Interrupt         */
} IRQn_Type;

/* --------  Configuration of the Cortex-A9 Processor and Core Peripherals  ------- */
#define __CA_REV         0x0000    /*!< Core revision r0       */

#include "core_ca.h"
#include "system_VE_A9_MP.h"


/******************************************************************************/
/*                Device Specific Peripheral Section                          */
/******************************************************************************/
/** @addtogroup VE_A9_MP_Peripherals VE_A9_MP Peripherals
  VE_A9_MP Device Specific Peripheral registers structures
  @{
*/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#include "pl310.h"
#include "gic.h"

#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

/*@}*/ /* end of group VE_A9_MP_Peripherals */


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/** @addtogroup VE_A9_MP_MemoryMap VE_A9_MP Memory Mapping
  @{
*/

/* Peripheral and RAM base address */
#define VE_A9_MP_FLASH_BASE0                  (0x00000000UL)                        /*!< (FLASH0    ) Base Address */
#define VE_A9_MP_FLASH_BASE1                  (0x08000000UL)                        /*!< (FLASH1    ) Base Address */
#define VE_A9_MP_PERIPH_BASE                  (0x18000000UL)                        /*!< (Peripheral) Base Address */
#define VE_A9_MP_SRAM_BASE                    (0x2E000000UL)                        /*!< (SRAM      ) Base Address */
#define VE_A9_MP_DRAM_BASE                    (0x80000000UL)                        /*!< (DRAM      ) Base Address */
#define VE_A9_MP_VRAM_BASE                    (0x18000000UL)                        /*!< (VRAM      ) Base Address */
#define VE_A9_MP_ETHERNET_BASE                (0x02000000UL + VE_A9_MP_PERIPH_BASE) /*!< (ETHERNET  ) Base Address */
#define VE_A9_MP_USB_BASE                     (0x03000000UL + VE_A9_MP_PERIPH_BASE) /*!< (USB       ) Base Address */
#define VE_A9_MP_DAP_BASE                     (0x1C000000UL)                        /*!< (DAP       ) Base Address */
#define VE_A9_MP_SYSTEM_REG_BASE              (0x00010000UL + 0x1C000000UL)         /*!< (SYSTEM REG) Base Address */
#define VE_A9_MP_SERIAL_BASE                  (0x00030000UL + 0x1C000000UL)         /*!< (SERIAL    ) Base Address */
#define VE_A9_MP_AACI_BASE                    (0x00040000UL + 0x1C000000UL)         /*!< (AACI      ) Base Address */
#define VE_A9_MP_MMCI_BASE                    (0x00050000UL + 0x1C000000UL)         /*!< (MMCI      ) Base Address */
#define VE_A9_MP_KMI0_BASE                    (0x00060000UL + 0x1C000000UL)         /*!< (KMI0      ) Base Address */
#define VE_A9_MP_UART_BASE                    (0x00090000UL + 0x1C000000UL)         /*!< (UART      ) Base Address */
#define VE_A9_MP_WDT_BASE                     (0x000F0000UL + 0x1C000000UL)         /*!< (WDT       ) Base Address */
#define VE_A9_MP_TIMER_BASE                   (0x00110000UL + 0x1C000000UL)         /*!< (TIMER     ) Base Address */
#define VE_A9_MP_DVI_BASE                     (0x00160000UL + 0x1C000000UL)         /*!< (DVI       ) Base Address */
#define VE_A9_MP_RTC_BASE                     (0x00170000UL + 0x1C000000UL)         /*!< (RTC       ) Base Address */
#define VE_A9_MP_UART4_BASE                   (0x001B0000UL + 0x1C000000UL)         /*!< (UART4     ) Base Address */
#define VE_A9_MP_CLCD_BASE                    (0x001F0000UL + 0x1C000000UL)         /*!< (CLCD      ) Base Address */
#define VE_A9_MP_GIC_DISTRIBUTOR_BASE         (0x00001000UL + 0x2C000000UL)         /*!< (GIC DIST  ) Base Address */
#define VE_A9_MP_GIC_INTERFACE_BASE           (0x00000100UL + 0x2C000000UL)         /*!< (GIC CPU IF) Base Address */
//The VE-A9 model implements L1 cache as architecturally defined, but does not implement L2 cache.
//Do not enable the L2 cache if you are running RTX on a VE-A9 model as it may cause a data abort.
#define VE_A9_MP_PL310_BASE                   (0x1E00A000UL)                        /*!< (PL310     ) Base Address */

//Following macros define the descriptors and attributes used to define the VE_A9 MMU flat-map
//Sect_Normal. Outer & inner wb/wa, non-shareable, executable, rw, domain 0.
#define section_normal(descriptor_l1, region)     region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = WB_WA; \
                                   region.outer_norm_t = WB_WA; \
                                   region.mem_t = NORMAL; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   __get_section_descriptor(&descriptor_l1, region);

//Sect_Normal_Cod. Outer & inner wb/wa, non-shareable, executable, ro, domain 0.
#define section_normal_cod(descriptor_l1, region) region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = WB_WA; \
                                   region.outer_norm_t = WB_WA; \
                                   region.mem_t = NORMAL; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = EXECUTE; \
                                   region.priv_t = READ; \
                                   region.user_t = READ; \
                                   region.sh_t = NON_SHARED; \
                                   __get_section_descriptor(&descriptor_l1, region);

//Sect_Normal_RO. Sect_Normal_Cod, but not executable
#define section_normal_ro(descriptor_l1, region)  region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = WB_WA; \
                                   region.outer_norm_t = WB_WA; \
                                   region.mem_t = NORMAL; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = READ; \
                                   region.user_t = READ; \
                                   region.sh_t = NON_SHARED; \
                                   __get_section_descriptor(&descriptor_l1, region);

//Sect_Normal_RW. Sect_Normal_Cod, but writeable and not executable
#define section_normal_rw(descriptor_l1, region) region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = WB_WA; \
                                   region.outer_norm_t = WB_WA; \
                                   region.mem_t = NORMAL; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   __get_section_descriptor(&descriptor_l1, region);
//Sect_SO. Strongly-ordered (therefore shareable), not executable, rw, domain 0, base addr 0
#define section_so(descriptor_l1, region) region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = NON_CACHEABLE; \
                                   region.outer_norm_t = NON_CACHEABLE; \
                                   region.mem_t = STRONGLY_ORDERED; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   __get_section_descriptor(&descriptor_l1, region);

//Sect_Device_RO. Device, non-shareable, non-executable, ro, domain 0, base addr 0
#define section_device_ro(descriptor_l1, region) region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = NON_CACHEABLE; \
                                   region.outer_norm_t = NON_CACHEABLE; \
                                   region.mem_t = STRONGLY_ORDERED; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = READ; \
                                   region.user_t = READ; \
                                   region.sh_t = NON_SHARED; \
                                   __get_section_descriptor(&descriptor_l1, region);

//Sect_Device_RW. Sect_Device_RO, but writeable
#define section_device_rw(descriptor_l1, region) region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = NON_CACHEABLE; \
                                   region.outer_norm_t = NON_CACHEABLE; \
                                   region.mem_t = STRONGLY_ORDERED; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   __get_section_descriptor(&descriptor_l1, region);
//Page_4k_Device_RW.  Shared device, not executable, rw, domain 0
#define page4k_device_rw(descriptor_l1, descriptor_l2, region) region.rg_t = PAGE_4k; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = NON_CACHEABLE; \
                                   region.outer_norm_t = NON_CACHEABLE; \
                                   region.mem_t = SHARED_DEVICE; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   __get_page_descriptor(&descriptor_l1, &descriptor_l2, region);

//Page_64k_Device_RW.  Shared device, not executable, rw, domain 0
#define page64k_device_rw(descriptor_l1, descriptor_l2, region)  region.rg_t = PAGE_64k; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = NON_CACHEABLE; \
                                   region.outer_norm_t = NON_CACHEABLE; \
                                   region.mem_t = SHARED_DEVICE; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   __get_page_descriptor(&descriptor_l1, &descriptor_l2, region);
#ifdef __cplusplus
}
#endif

#endif  // __VE_A9_MP_H__
