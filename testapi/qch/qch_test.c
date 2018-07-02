/********************************************************
 * Copyright (c) 2017 by Sunplus Technology Co., Ltd.
 *    ____               __
 *   / __/_ _____  ___  / /_ _____
 *  _\ \/ // / _ \/ _ \/ / // (_-<
 * /___/\_,_/_//_/ .__/_/\_,_/___/
 *              /_/
 * Sunplus Technology Co., Ltd.19, Innovation First Road,
 * Science-Based Industrial Park, Hsin-Chu, Taiwan, R.O.C.
 * ------------------------------------------------------
 *
 * Description :  q channel test
 * ------------------------------------------------------
 * Rev  Date          Author(s)      Status & Comments
 * ======================================================
 * 0.1  2017/09/12    nz.lu          initial version
 */
#include <emuio.h>
#include <string.h>
#include "sp_interrupt.h"

#include "regmap_8388_all.h"
#include "iop_car.inc"


#define TICKS	(4500-1)	/* 50ms */
#define ENABLE	(1)
#define DISABLE	(0)

#define ON	(1)
#define OFF	(0)

#define REG_BASE	(0x9c000000)
#define RF_GRP(_grp, _reg) ((((_grp) * 32 + (_reg)) * 4) + REG_BASE)

#define CBDMA0_REG	((volatile struct cbdma_regs *)RF_GRP(26, 0))
#define CBDMA1_REG	((volatile struct cbdma_regs *)RF_GRP(27, 0))

#define TEST_SIZE				(8 << 20)
#define CBDMA_TEST_SOURCE		(0x0600000)
#define CBDMA_TEST_DESTINATION	(CBDMA_TEST_SOURCE + TEST_SIZE)

#define CBDMA0_SRAM_BASE ((unsigned char *) 0x9E800000)	// 0x9e800000~0x9e81ffff 64KB)
#define BOOT_ROM_BASE ((unsigned char *) 0xFFFF0000)

#define TIMER3_TICKS		(90 - 1)		/* 1ms */
#define TIMER3_CONFIG_STC	(1 << 2)	/* src: stc */
#define TIMER3_RELOAD		(1 << 1)	/* timer3 auto reload */
#define TIMER3_RUN 			(1 << 0)	/* timer3 run */
#define TIMER3_STOP			(0 << 0)	/* timer3 stop */

unsigned int is_cbdma0_done = FALSE;
unsigned int clock_flipflop = 0;
unsigned int flipflop_cnt = 0;
unsigned int cbdma_clock_onoff = FALSE;

interrupt_operition timer3_intr_cfg;
interrupt_operition cbdma0_intr_cfg;


void timer_intr_onoff(int onoff)
{
	if (onoff == ENABLE) {
		/* enable timer0 interrupt */
		hal_interrupt_unmask(154);
	} else {
		hal_interrupt_mask(154);
	}
}


void cbdma0_intr_onoff(int onoff)
{
	if (onoff == ENABLE) {
		hal_interrupt_unmask(128);
	} else {
		hal_interrupt_mask(128);
	}
}

enum enum_qchannel_id {
	QCHANNEL_ID_DISPLAY_DATA_FETCH = 0,
	QCHANNEL_ID_BOOT_ROM,
	QCHANNEL_ID_CBDMA0,
	QCHANNEL_ID_MAX
};

#define ACCEPT (1 << 1)
#define ACTIVE (1 << 2)
#define DENY   (1 << 3)

const unsigned int qchannel_offset_list[QCHANNEL_ID_MAX] = {15, 16, 17};

int __attribute__((optimize("O0"))) qch_access_protect_routine(unsigned int qid,unsigned int onoff)
{
	volatile struct moon1_regs *moon1 = MOON1_REG;
	unsigned int offset = qchannel_offset_list[qid];
	unsigned int status;
	int retry_cnt = 200, err_code = -1;

	if (onoff == ON) {
		moon1->sft_cfg[offset] = 1;
		/**
		 * wait until accept bit == 1
		 * |         | ACCEPT | DENY |
		 * |---------|--------|------|
		 * | Default |      0 |    0 |
		 * | Success |      1 |    0 |
		 * | Fail    |      0 |    1 |
		 */
		while (retry_cnt-- > 0) {
			status = moon1->sft_cfg[offset];
			if ((status & ACCEPT) != 0) {
				printf("Accept. 0x%x\n", moon1->sft_cfg[offset]);
				err_code = 0;
				break;
			}
			if ((status & DENY) != 0) {
				printf("Deny. 0x%x\n", moon1->sft_cfg[offset]);
				break;
			}
		}
	} else {
		moon1->sft_cfg[offset] = 0;
		/**
		 * wait until accept bit == 0
		 * |         | ACCEPT | DENY |
		 * |---------|--------|------|
		 * | Default |      1 |    0 |
		 * | Success |      0 |    0 |
		 * | Fail    |      0 |    1 |
		 */
		while (retry_cnt-- > 0) {
			status = moon1->sft_cfg[offset];
			if ((status & ACCEPT) == 0) {
				printf("Accept. 0x%x\n", moon1->sft_cfg[offset]);
				err_code = 0;
				break;
			}
			if ((status & DENY) != 0) {
				printf("Deny. 0x%x\n", moon1->sft_cfg[offset]);
				break;
			}
		}
	}

	if (retry_cnt <= 0) {
		printf("Wait ACCEPT timeout. 0x%x\n", moon1->sft_cfg[offset]);
	}

	if (err_code != 0) {
		moon1->sft_cfg[offset] = (onoff == ON) ? 0: 1;
	}

	return err_code;
}


void qch_timer_callback(void)
{
	hal_interrupt_acknowledge(154);
	volatile struct moon0_regs *moon0 = MOON0_REG;
	int rtn;

	if (cbdma_clock_onoff == TRUE) {
		if (clock_flipflop) {
			printf("@on (%d)\n", flipflop_cnt);
			moon0->clken[1] |= 1;
			rtn = qch_access_protect_routine(QCHANNEL_ID_CBDMA0, ON);
			if (rtn != 0) {
				moon0->clken[1] &= 0xFFFFFFFE;
			}
		} else {
			printf("@off(%d)\n", flipflop_cnt);
			rtn = qch_access_protect_routine(QCHANNEL_ID_CBDMA0, OFF);
			if (rtn == 0) {
				moon0->clken[1] &= 0xFFFFFFFE;
				flipflop_cnt++;
			}
		}

		if (rtn == 0)
			clock_flipflop = ~clock_flipflop;
	}
}


void cbdma_cfg_memcpy(int id, void *dst, void *src, unsigned int length)
{
	printf("[CBDMA:%d]: Copy %d KB from 0x%08x to 0x%08x\n", id, length>>10, (unsigned) src, (unsigned)dst);
	volatile struct cbdma_regs *cbdma;
	if (id)
		cbdma = CBDMA1_REG;
	else
		cbdma = CBDMA0_REG;

	// clear all int status
	cbdma->cbdma_int_status = 0x7f;
	// set copy mode
	cbdma->cbdma_config = 0x00030003;
	// set write data length
	cbdma->cbdma_dma_length = length;
	cbdma->cbdma_src_adr = (unsigned) src;
	cbdma->cbdma_des_adr = (unsigned) dst;
}


void cbdma_cfg_memset(int id, void *dst, unsigned length)
{
	volatile struct cbdma_regs *cbdma;
	if (id)
		cbdma = CBDMA1_REG;
	else
		cbdma = CBDMA0_REG;

	// clear all int status
	cbdma->cbdma_int_status = 0x7f;
	// set memset mode
	cbdma->cbdma_config = 0x00030000;
	// set write data length
	cbdma->cbdma_dma_length = length;
	cbdma->cbdma_src_adr = 0;
	cbdma->cbdma_des_adr = (unsigned) dst;
}


void cbdma_enable_int(int id)
{
	volatile struct cbdma_regs *cbdma;
	if (id)
		cbdma = CBDMA1_REG;
	else
		cbdma = CBDMA0_REG;
	cbdma->cbdma_int_en = 1;
}


void cbdma0_isr(void)
{
	hal_interrupt_acknowledge(128);

	printf("[CBDMA_%d]: Finished, status: 0x%x\n", 0, cbdma_get_int_status(0));
	cbdma_clear_int_status(0);
	is_cbdma0_done = TRUE;
}


void timer3_isr_cfg()
{
	printf("[CFG] Timer3\n");
	STC_REG->timer3_ctl = TIMER3_CONFIG_STC | TIMER3_RELOAD | TIMER3_RUN;
	STC_REG->timer3_pres_val = 0;
	STC_REG->timer3_reload = TIMER3_TICKS;
	STC_REG->timer3_cnt = TIMER3_TICKS;
	hal_interrupt_configure(154, 0, 1);
}


void cbdma0_isr_cfg()
{
	printf("[CFG] CBDMA0 ISR\n");
	cbdma_clear_int_status(0);
	cbdma_enable_int(0);
	hal_interrupt_configure(128, 0, 1);
	hal_interrupt_acknowledge(128);
}


void qch_initial_settings()
{
	MOON1_REG->sft_cfg[13] = 0x01afffff;	//set sdramA size [22:21], 00:128MB, 01:256MB, 10:512MB, 11:1024MB

	memcpy(timer3_intr_cfg.dev_name, "Timer3", strlen("Timer3"));
	timer3_intr_cfg.vector = 154;
	timer3_intr_cfg.device_config = timer3_isr_cfg;
	timer3_intr_cfg.interrupt_handler = qch_timer_callback;
	interrupt_register(&timer3_intr_cfg);

	memcpy(cbdma0_intr_cfg.dev_name, "CBDMA0", strlen("CBDMA0"));
	cbdma0_intr_cfg.vector = 128;
	cbdma0_intr_cfg.device_config = cbdma0_isr_cfg;
	cbdma0_intr_cfg.interrupt_handler = cbdma0_isr;
	interrupt_register(&cbdma0_intr_cfg);

	unsigned char *IOP_base =0x10000;
	int i;

	regs0->iop_control |= 0x1;

	memset(IOP_base, 0x0, sizeof(IOPcode_NEC));
	printf("IOP code size 0x%x\n", sizeof(IOPcode_NEC));
	memcpy(IOP_base, &IOPcode_NEC, sizeof(IOPcode_NEC));

	regs0->iop_control &=~(0x8000);
	regs0->iop_control &=~(0x200);

	regs0->iop_base_adr_l = ((UINT32)IOP_base & 0xFFFF);
	regs0->iop_base_adr_h = ((UINT32)IOP_base >> 16);

	regs0->memory_bridge_control |= 0x3;
	for (i = 0; i < 12; i++)
		regs0->iop_data[i] = 0;

	regs0->iop_control &= ~(0x01);
	// open the door!
	regs0->iop_data[1] = 0xaa;

}


void set_sequential_pattern(unsigned int *dst, unsigned int len)
{
	unsigned int *ptr = dst;
	unsigned int *end_ptr = (unsigned int *) ((unsigned int)dst + len);
	unsigned int pattern = 0;

	unsigned int check_repeat = 0, i;
	for (i = 0; i < 32; i++) {
		if (ptr[i] == i) {
			check_repeat++;
			if (check_repeat > 16)
				return;
		} else {
			break;
		}
	}

	printf("[set sequential pattern to 0x%08x]: start.\n", (unsigned int)ptr);
	while (ptr < end_ptr) {
		*ptr = pattern;
		pattern++;
		ptr++;
	}

	printf("[set sequential pattern]: set pattern done.\n");
	// printf("[set sequential pattern]: flush dcache.\n");
	// flush_dcache_all();
	// printf("[set sequential pattern]: done.\n");
}





void qch_host_test(int test_id)
{
	volatile struct moon0_regs *moon0 = MOON0_REG;
	unsigned int repeat_untill_fail = FALSE;
	unsigned int err = 0, retry_cnt = 0, is_timeout = FALSE;
	int repeat_cnt = -1, rtn;

	printf("[DBG] test_id : %d\n", test_id);

	/**
	 * strange, why enable clken before checking ?
	 */
	if ((moon0->clken[1] & 0x1) == 0) {
		moon0->clken[1] |= 1;
	}

	printf("[DBG] Now, Qchannel is running state.\n");
	rtn = qch_access_protect_routine(QCHANNEL_ID_CBDMA0, ON);
	if (rtn != 0) {
		printf("[DBG] Open CBDMA0 Fail.\n");
		return;
	}

	if (test_id == 0) {
		/**
		 * one shot and flip cbdma0 clock_en
		 * untill copy procedure finished.
.		 */
		cbdma_clock_onoff = TRUE;
	}

	if (test_id == 1) {
		/**
		 * 10 shots without flip cbdma0 clock_en.
		 */
		repeat_cnt = 0;
	}

	if (test_id == 2) {
		/**
		 * N shots and flip cbdma0 clock_en
		 * untill error occurred
		 */
		cbdma_clock_onoff = TRUE;
		repeat_untill_fail = TRUE;
	}

	// case 0
	// set_sequential_pattern((unsigned int *)CBDMA_TEST_SOURCE, TEST_SIZE);

__RETRY__:
	is_cbdma0_done = FALSE;
	clock_flipflop = 0;

	cbdma_cfg_memcpy(0, (void *)CBDMA_TEST_DESTINATION, (void *)CBDMA_TEST_SOURCE, TEST_SIZE);
	cbdma0_intr_onoff(ENABLE);
	cbdma_kick_go(0);
	timer_intr_onoff(ENABLE);

	is_timeout = FALSE;
	while (is_cbdma0_done == FALSE) {
		if (flipflop_cnt > 32) {
			printf("Time out\n");
			is_timeout = TRUE;
			break;
		}
	}
	timer_intr_onoff(DISABLE);
	cbdma0_intr_onoff(DISABLE);

	// if (is_timeout == FALSE)
	// 	err = check_sequential_pattern((unsigned int *)CBDMA_TEST_DESTINATION, TEST_SIZE);

	flipflop_cnt = 0;

	if (repeat_untill_fail == TRUE) {
		if (err == 0 && is_timeout != TRUE) {
			retry_cnt++;
			printf("[DBG] retry_cnt : %d\n", retry_cnt);
			goto __RETRY__;
		}
	}

	if (repeat_cnt-- > 0) {
		retry_cnt++;
		printf("[DBG] retry_cnt : %d\n", retry_cnt);
		goto __RETRY__;
	}

	cbdma_clock_onoff = FALSE;

	rtn = qch_access_protect_routine(QCHANNEL_ID_CBDMA0, OFF);
	if (rtn != 0) {
		printf("[DBG] Close CBDMA0 Fail.\n");
		return;
	}
	moon0->clken[1] &= 0xFFFFFFFE;

	printf("Done~\n");
}

void load_n_byte_from_cbdma_sram(unsigned int load_len)
{
	char idx;
	volatile unsigned char *dptr = CBDMA0_SRAM_BASE;
	printf("dump position: 0x%08x\n", dptr);
	for(idx = 0; idx < load_len; idx++)
	{
		if (idx != 0 && idx % 16 == 0)
			printf("\n");
		printf("%02x ", *(dptr + idx ));
	}
	printf("\n");
}


void qch_device_lead_test(int test_id)
{
	volatile struct moon0_regs *moon0 = MOON0_REG;
	int rtn;

	// disable cbdma clock
	rtn = qch_access_protect_routine(QCHANNEL_ID_CBDMA0, OFF);
	if (rtn != 0) {
		printf("[DBG] Close CBDMA0 Fail.\n");
		return;
	}
	moon0->clken[1] &= 0xFFFFFFFE;
	printf("Close cbdma clock done.\n");

	// try to access sram in cbdma.
	load_n_byte_from_cbdma_sram(32);

	rtn = qch_access_protect_routine(QCHANNEL_ID_CBDMA0, OFF);
	if (rtn != 0) {
		printf("[DBG] Close CBDMA0 Fail.\n");
		return;
	}
	moon0->clken[1] &= 0xFFFFFFFE;

}



