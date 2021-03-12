#include <types.h>
#include <common.h>
#include <config.h>

#include "sp_interrupt.h"
#include "common_all.h"
#include "cache.h"
#include "stc.h"
#include "gpio_drv.h"

#define A_and_B_chip   //A and B chip running simultaneously
//#define A_chip_only       //A chip only

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


#ifdef I2C_TEST
u8	data_buf[255];
u8	tx_buf[255];
#endif


//#define INTR_SAMPLE
#ifdef INTR_SAMPLE
void gpio_int_0_callback(void)
{
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

#ifdef A_chip_only
int main(void)
{

#ifdef I2C_TEST
    unsigned int test;
#endif

	printf("Build @%s, %s\n", __DATE__, __TIME__);

	hw_init();
	AV1_STC_init();
	
	/*initial interrupt vector table*/
	int_memcpy(0x00000000, __vectors_start, (unsigned)__vectors_end - (unsigned)__vectors_start);

	mmu_init();
	HAL_DCACHE_ENABLE();

	ipc_init();
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

#ifdef RS485_TEST
	AV1_STC_init();
	rs485_init(10,11,12);	//G_MX[10]_TX --> DI, G_MX[11]_RX --> RO ,G_MX[12]_RTS
#endif 

	/* interrupt manager module init */
	sp_interrupt_setup();
	ipc_start();

#ifdef I2C_TEST
	sp_i2c_master_init();
	sp_i2c_master_set_freq_khz(0, 100);
#endif

	printf("NonOS boot OK!!!\n");
	task_dbg();
	while(1);

	//Never get here
	return 0;
}
#endif 

#ifdef A_and_B_chip
void uart2_init()
{
	MOON3_REG->sft_cfg[16] = RF_MASK_V((0x7f << 0), (11 << 0));
	MOON3_REG->sft_cfg[16] = RF_MASK_V((0x7f << 8), (15 << 8));
	MOON0_REG->reset[1] = RF_MASK_V_CLR(1 << 10); /* release UA1 */
	UART2_REG->div_l = UART_BAUD_DIV_L(BAUDRATE, UART_SRC_CLK);
	UART2_REG->div_h = UART_BAUD_DIV_H(BAUDRATE, UART_SRC_CLK);
}

void uart_isr_init(void)
{
	UART_REG->isc = (1 << 5); // RX interrupt enable
	interrupt_register(55, "UART2", uart_isr, 1);
}

int main(void)
{
#if 0 // MALLOC_TEST
	void *p1 = malloc(1 * 1024 * 1024);
	void *p2 = malloc(2 * 1024 * 1024);
	printf("MALLOC_TEST: %08x %08x\n", p1 , p2);
	free(p1);
	free(p2);
#endif

#ifdef I2C_TEST
    unsigned int test;
#endif

	hw_init();
	AV1_STC_init();
	
	/*initial interrupt vector table*/
	int_memcpy(0x00000000, __vectors_start, (unsigned)__vectors_end - (unsigned)__vectors_start);
	sp_interrupt_setup();

	//timer_test_init();
	//cbdma_test_init();
	uart2_init();
	uart_isr_init();

#ifdef RS485_TEST
	rs485_init(10,11,12);	//G_MX[10]_TX --> DI, G_MX[11]_RX --> RO ,G_MX[12]_RTS
#endif 

	//timer_test();

	printf("NonOS boot OK!!!\n");
	//task_dbg();

	Marlin_main();
	while(1);

	//Never get here
	return 0;
}
#endif 
