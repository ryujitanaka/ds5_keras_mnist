/**************************************************************************//**
 * @file     mmu_Renesas_RZ_A1.c
 * @brief    MMU Startup File for mmu_Renesas_RZ_A1 Device Series
 * @version  V1.01
 * @date     30 Oct 2013
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2011 - 2013 ARM LIMITED

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

/* Memory map description from: Renesas RZ_A1H_05E_121130.pdf

                                                     Memory Type
0xffffffff |--------------------------|             ------------
           |        Peripherals       |                Device
0xfcf00000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0xe8300000 |--------------------------|             ------------
           |        Peripherals       |                Device
0xe8000000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x60A00000 |--------------------------|             ------------
           | On Chip RAM (10M) Mirror |                Fault
0x60000000 |--------------------------|             ------------
           |  SPI multi I/O 64MB      |                Fault
0x5c000000 |--------------------------|             ------------
           |  SPI multi I/O 64MB      |                Fault
0x58000000 |--------------------------|             ------------
           |        CS5 Mirror        |                Fault
0x54000000 |--------------------------|             ------------
           |        CS4 Mirror        |                Fault
0x50000000 |--------------------------|             ------------
           |        CS3 Mirror        |                Fault
0x4c000000 |--------------------------|             ------------
           |        CS2 Mirror        |                Fault
0x48000000 |--------------------------|             ------------
           |        CS1 Mirror        |                Fault
0x44000000 |--------------------------|             ------------
           |        CS0 Mirror        |                Fault
0x40000000 |--------------------------|             ------------
           |          BSC             |                 RW
0x3ff00000 |--------------------------|             ------------
           |      SPI_MIO_BASE        |                 RW
0x3fe00000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x20A00000 |--------------------------|             ------------
           |    On Chip RAM (10M)     |                 RW
0x20000000 |--------------------------|             ------------
           |  SPI multi I/O 64MB      |                 RO
0x1c000000 |--------------------------|             ------------
           |  SPI multi I/O 64MB      |                 RO
0x18000000 |--------------------------|             ------------
           |  CS5 User Area 64MB      |                 RW
0x14000000 |--------------------------|             ------------
           |  CS4 User Area 64MB      |                 RW
0x10000000 |--------------------------|             ------------
           |      CS3 SDRAM 64MB      |                 RW
0x0c000000 |--------------------------|             ------------
           |      CS2 SDRAM 64MB      |                 RW
0x08000000 |--------------------------|             ------------
           |  CS1 NOR Flash 64MB      |                 RO
0x04000000 |--------------------------|             ------------
           |  CS0 NOR Flash 64MB      |                 RO
0x00000000 |--------------------------|             ------------
*/

#define Renesas_RZ_A1_SPI_MIO_BASE                 (0x3fe00000UL)                        /*!< (SPI_MIO   ) Base Address */
#define Renesas_RZ_A1_BSC_BASE                     (0x3ff00000UL)                        /*!< (BSC       ) Base Address */
#define Renesas_RZ_A1_PERIPH_BASE0                 (0xe8000000UL)                        /*!< (PERIPH0   ) Base Address */
#define Renesas_RZ_A1_PERIPH_BASE1                 (0xfcf00000UL)                        /*!< (PERIPH1   ) Base Address */
// L1 Cache info and restrictions about architecture of the caches (CCSIR register):
// Write-Through support *not* available
// Write-Back support available.
// Read allocation support available.
// Write allocation support available.

//Note: You should use the Shareable attribute carefully.
//For cores without coherency logic (such as SCU) marking a region as shareable forces the processor to not cache that region regardless of the inner cache settings.
//Cortex-A versions of RTX use LDREX/STREX instructions relying on Local monitors. Local monitors will be used only when the region gets cached, regions that are not cached will use the Global Monitor.
//Some Cortex-A implementations do not include Global Monitors, so wrongly setting the attribute Shareable may cause STREX to fail.

//Recall: When the Shareable attribute is applied to a memory region that is not Write-Back, Normal memory, data held in this region is treated as Non-cacheable.
//When SMP bit = 0, Inner WB/WA Cacheable Shareable attributes are treated as Non-cacheable.
//When SMP bit = 1, Inner WB/WA Cacheable Shareable attributes are treated as Cacheable.


//Following MMU configuration is expected
//SCTLR.AFE == 1 (Simplified access permissions model - AP[2:1] define access permissions, AP[0] is an access flag)
//SCTLR.TRE == 0 (TEX remap disabled, so memory type and attributes are described directly by bits in the descriptor)
//Domain 0 is always the Client domain
//Descriptors place all memory in domain 0
//There are no restrictions by privilege level (PL0 can access all memory)

#include <stdint.h>
#include "Renesas_RZ_A1.h"

//Import symbols from linker
extern uint32_t Image$$VECTORS$$Base;
extern uint32_t Image$$RW_DATA$$Base;
extern uint32_t Image$$ZI_DATA$$Base;
extern uint32_t Image$$TTB$$ZI$$Base;

static uint32_t Sect_Normal;     //outer & inner wb/wa, non-shareable, executable, rw, domain 0, base addr 0
static uint32_t Sect_Normal_Cod; //outer & inner wb/wa, non-shareable, executable, ro, domain 0, base addr 0
static uint32_t Sect_Normal_RO;  //as Sect_Normal_Cod, but not executable
static uint32_t Sect_Normal_RW;  //as Sect_Normal_Cod, but writeable and not executable
static uint32_t Sect_Device_RO;  //device, non-shareable, non-executable, ro, domain 0, base addr 0
static uint32_t Sect_Device_RW;  //as Sect_Device_RO, but writeable

/* Define global descriptors */
static uint32_t Page_L1_4k  = 0x0;  //generic
static uint32_t Page_L1_64k = 0x0;  //generic
static uint32_t Page_4k_Device_RW;  //Shared device, not executable, rw, domain 0
static uint32_t Page_64k_Device_RW; //Shared device, not executable, rw, domain 0

void create_translation_table(void)
{
    mmu_region_attributes_Type region;

    /*
     * Generate descriptors. Refer to Renesas_RZ_A1.h to get information about attributes
     *
     */
    //Create descriptors for Vectors, RO, RW, ZI sections
    section_normal(Sect_Normal, region);
    section_normal_cod(Sect_Normal_Cod, region);
    section_normal_ro(Sect_Normal_RO, region);
    section_normal_rw(Sect_Normal_RW, region);
    //Create descriptors for peripherals
    section_device_ro(Sect_Device_RO, region);
    section_device_rw(Sect_Device_RW, region);
    //Create descriptors for 64k pages
    page64k_device_rw(Page_L1_64k, Page_64k_Device_RW, region);
    //Create descriptors for 4k pages
    page4k_device_rw(Page_L1_4k, Page_4k_Device_RW, region);

    /*
     *  Define MMU flat-map regions and attributes
     *
     */

    //Create 4GB of faulting entries
    __TTSection (&Image$$TTB$$ZI$$Base, 0, 4096, DESCRIPTOR_FAULT);

    // R7S72100 memory map.
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_NORFLASH_BASE0    , 64, Sect_Normal_RO);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_NORFLASH_BASE1    , 64, Sect_Normal_RO);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_SDRAM_BASE0       , 64, Sect_Normal_RW);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_SDRAM_BASE1       , 64, Sect_Normal_RW);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_USER_AREA0        , 64, Sect_Normal_RW);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_USER_AREA1        , 64, Sect_Normal_RW);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_SPI_IO0           , 64, Sect_Normal_RO);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_SPI_IO1           , 64, Sect_Normal_RO);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_ONCHIP_SRAM_BASE  , 10, Sect_Normal_RW);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_SPI_MIO_BASE      ,  1, Sect_Device_RW);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_BSC_BASE          ,  1, Sect_Device_RW);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_PERIPH_BASE0      ,  3, Sect_Device_RW);
    __TTSection (&Image$$TTB$$ZI$$Base, Renesas_RZ_A1_PERIPH_BASE1      , 49, Sect_Device_RW);

    //Define Image
    __TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)&Image$$VECTORS$$Base, 1, Sect_Normal_Cod);
    __TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)&Image$$RW_DATA$$Base, 1, Sect_Normal_RW);
    __TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)&Image$$ZI_DATA$$Base, 1, Sect_Normal_RW);

    /* Set location of level 1 page table
    ; 31:14 - Translation table base addr (31:14-TTBCR.N, TTBCR.N is 0 out of reset)
    ; 13:7  - 0x0
    ; 6     - IRGN[0] 0x0 (Inner WB WA)
    ; 5     - NOS     0x0 (Non-shared)
    ; 4:3   - RGN     0x1 (Outer WB WA)
    ; 2     - IMP     0x0 (Implementation Defined)
    ; 1     - S       0x0 (Non-shared)
    ; 0     - IRGN[1] 0x1 (Inner WB WA) */
    __set_TTBR0(((uint32_t)&Image$$TTB$$ZI$$Base) | 9);

    /* Set up domain access control register
    ; We set domain 0 to Client and all other domains to No Access.
    ; All translation table entries specify domain 0 */
    __set_DACR(1);
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
