#include "sp70xx.h"
#include "sp7021_hal_conf.h"
#include "mem_sp7021.h"


uint32_t SystemCoreClock = HSI_VALUE;
extern uint8_t __vectors_start[];
extern uint8_t __vectors_end[];






static uint32_t Sect_Normal;	 
static uint32_t Sect_Normal_Cod; 
static uint32_t Sect_Device_RW; //outer & inner wb/wa,	ro, domain 0, base addr 0

extern void* Image$$TTB$$ZI$$Base;
extern void* __etext;
extern void* Image$$TTB$$ZI$$Limit;


static void MMU_CreateTranslationTable(void)
{
	mmu_region_attributes_Type region;
	uint32_t domain_access_mode = 0xFFFFFFFF;
	int i  = 0; 
	uint32_t* pdata = NULL;

	//printf("%s, __etext = 0x%p, __etext = 0x%08x\n", __FUNCTION__, __etext, &__etext);
	//printf("%s, Image$$TTB$$ZI$$Base = 0x%p, Image$$TTB$$ZI$$Base = 0x%08x\n", __FUNCTION__, Image$$TTB$$ZI$$Base, &Image$$TTB$$ZI$$Base);
	//printf("%s, Image$$TTB$$ZI$$Limit = 0x%p, Image$$TTB$$ZI$$Limit = 0x%08x\n", __FUNCTION__, Image$$TTB$$ZI$$Limit, &Image$$TTB$$ZI$$Limit);
	MMU_TTSection ((uint32_t*)&Image$$TTB$$ZI$$Base, 0, 4096, DESCRIPTOR_FAULT);
	section_normal(Sect_Normal, region);
	section_device_rw(Sect_Device_RW, region);

	MMU_TTSection ((uint32_t*)&Image$$TTB$$ZI$$Base, __RAM_BASE, __RAM_SIZE/0x100000, Sect_Normal); // RAM
	MMU_TTSection ((uint32_t*)&Image$$TTB$$ZI$$Base, __SHMEM_BASE, __SHMEM_SIZE/0x100000, Sect_Device_RW); // openamp sharemem, NC
	//MMU_TTSection ((uint32_t*)&Image$$TTB$$ZI$$Base, __SPI_NOR_BASE, __SPI_NOR_SIZE/0x100000, Sect_Normal); // SPI_NOR
	MMU_TTSection ((uint32_t*)&Image$$TTB$$ZI$$Base, __DEVICE_BASE, __DEVICE_SIZE/0x100000, Sect_Device_RW); // DEVICE, NC
	//MMU_TTSection ((uint32_t*)&Image$$TTB$$ZI$$Base, __ROM_BASE, __ROM_SIZE/0x100000, Sect_Normal); // ROM

#if 0
	pdata =(uint32_t*)&Image$$TTB$$ZI$$Base;
	for (i = 0; i < 4096;i++)
	{
		printf("0x%08x ", *(pdata+i));

		if ((i+1)%16 == 0)
			printf("\n");
	}
  #endif
	  __set_TTBR0((uint32_t*)&Image$$TTB$$ZI$$Base);
    __ISB();

    /* Set up domain access control register
    ; We set domain 0 to Client and all other domains to No Access.
    ; All translation table entries specify domain 0 */
    __set_DACR(0xFFFFFFFF);
    __ISB();
}



static inline void *hal_memcpy(void *dest, const void *src, size_t n)
{
    uint8_t *pdest = (uint8_t*)dest;
    uint8_t *psrc = (uint8_t*)src;

	while(n--){
        *pdest++ = *psrc++;
    }

	return dest;
}

void SystemInit (void)
{
	/*system  init*/
#if 1
	L1C_DisableCaches();
	__set_TLBIALL(0);
  	__set_ICIALLU(0);
	L1C_InvalidateDCacheAll();
	MMU_CreateTranslationTable();
	MMU_Enable();
	//Enable Caches
	L1C_EnableCaches();
	
#endif	
   /*Interrupt  vector init*/	
    hal_memcpy(0x00000000, __vectors_start, (unsigned)__vectors_end - (unsigned)__vectors_start);
	IRQ_Initialize();
}


void SystemCoreClockUpdate (void)
{
	SystemCoreClock = HAL_PLL_Get_ARM926Freq();

}


