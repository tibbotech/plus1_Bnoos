#include <types.h>
#include <common.h>
#include <config.h>

#include "sp_interrupt.h"
#include "common_all.h"
#include "cache.h"

#ifdef NOC_TEST
// #include "display_pattern_384x240_nv12.inc"
extern void noc_initial_settings();
const unsigned char longlonglong_table[16];
#endif

#ifdef QCH_TEST
extern void qch_initial_settings();
#endif

extern void mmu_init();

extern char __vectors_start[];
extern char __vectors_end[];

//#define INTR_SAMPLE
#ifdef INTR_SAMPLE
void gpio_int_0_callback(void)
{
	hal_interrupt_acknowledge(120);
	printf("GPIO_INT_0\n");

}
void gpio_int_0_isr_cfg()
{
	volatile unsigned int *sft_cfg3 = (unsigned int *)0x9c000180;
	printf("[CFG] GPIO_INT_0\n");
	sft_cfg3[24] = 0x200020;
	hal_interrupt_configure(120, 1, 1);
	hal_interrupt_unmask(120);
}
void gpio_intr_test_init()
{
	static interrupt_operation gpio_int_0;

	memcpy(gpio_int_0.dev_name, "GP_IN0", strlen("GP_IN0"));

	gpio_int_0.vector = 120;
	gpio_int_0.device_config = gpio_int_0_isr_cfg;
	gpio_int_0.interrupt_handler = gpio_int_0_callback;

	interrupt_register(&gpio_int_0);
}
#endif

void hw_init()
{
	unsigned int i;

	/* clken[all]  = enable */
	for (i = 0; i < sizeof(MOON0_REG->clken) / 4; i++)
		MOON0_REG->clken[i] = RF_MASK_V_SET(0xffff);
	/* gclken[all] = no */
	for (i = 0; i < sizeof(MOON0_REG->gclken) / 4; i++)
		MOON0_REG->gclken[i] = RF_MASK_V_CLR(0xffff);
	/* reset[all] = clear */
	for (i = 0; i < sizeof(MOON0_REG->reset) / 4; i++)
		MOON0_REG->reset[i] = RF_MASK_V_CLR(0xffff);

}

#define IPC_B2A_TEST
#ifdef IPC_B2A_TEST
#define IPC_A2B		(0x9c008100) // G258
#define IPC_B2A		(0x9c008180) // G259
#define CA7_READY	(0xca700001)

static void ipc_b2a_test(void)
{
	volatile unsigned int *a2b = (volatile unsigned int *)IPC_A2B;
	volatile unsigned int *b2a = (volatile unsigned int *)IPC_B2A;

	printf("IPC test:\nwait A ready...\n");
	while (a2b[31] != CA7_READY);

	printf("test B2A...\n");
	// direct (mbox)
	b2a[24] = 0x12345678;
	b2a[25] = 0x5a5a5a5a;
	b2a[26] = 0xa5a5a5a5;
	b2a[27] = 0xdeadc0de;
	b2a[28] = 0x01010101;
	b2a[29] = 0x19730611;
	b2a[30] = 0x87654321;
	b2a[31] = 0x00000000;
	// rpc
	b2a[0] = 1;
}
#endif

int main(void)
{
	printf("Build @%s, %s\n", __DATE__, __TIME__);

	hw_init();

	/*initial interrupt vector table*/
	int_memcpy(0x00000000, __vectors_start, (unsigned)__vectors_end - (unsigned)__vectors_start);

	mmu_init();
	HAL_DCACHE_ENABLE();


#ifdef NOC_TEST
	noc_initial_settings();
#endif

#ifdef QCH_TEST
	qch_initial_settings();
#endif

	//cbdma_test_init();
	timer_test_init();
#ifdef AXI_MON
	axi_mon_test_init();
#endif

#ifdef INTR_SAMPLE
	gpio_intr_test_init();
#endif
	/* interrupt manager module init */
	sp_interrupt_setup();

	printf("NonOS boot OK!!!\n");

#ifdef IPC_B2A_TEST
	ipc_b2a_test();
#endif

	task_dbg();

	while(1);

	//Never get here
	return 0;
}
