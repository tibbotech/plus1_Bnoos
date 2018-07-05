/********************************************************
 * Copyright (c) 2018 by Sunplus Technology Co., Ltd.
 *    ____               __
 *   / __/_ _____  ___  / /_ _____
 *  _\ \/ // / _ \/ _ \/ / // (_-<
 * /___/\_,_/_//_/ .__/_/\_,_/___/
 *              /_/
 * Sunplus Technology Co. 19, Innovation First Road,
 * Science-Based Industrial Park, Hsin-Chu, Taiwan, R.O.C.
 * ------------------------------------------------------
 *
 * Description :  simple timer driver
 * ------------------------------------------------------
 * Rev  Date          Author(s)      Status & Comments
 * ======================================================
 * 0.1  2018/06/28    nz.lu          initial version
 */
#include "common_all.h"
#include "cache.h"
#include "sp_interrupt.h"




#define TIMER3_TICKS		(90 - 1)		/* 1ms */
#define TIMER3_CONFIG_STC	(1 << 2)	/* src: stc */
#define TIMER3_RELOAD		(1 << 1)	/* timer3 auto reload */
#define TIMER3_RUN 			(1 << 0)	/* timer3 run */
#define TIMER3_STOP			(0 << 0)	/* timer3 stop */


static unsigned int g_repeat_cnt = 0;


void timer3_interrupt_control_mask(int enable)
{
	if (enable != 0) {
		/* enable timer0 interrupt */
		hal_interrupt_unmask(154);
	} else {
		hal_interrupt_mask(154);
	}
}



static void timer3_isr_cfg()
{
	printf("[CFG] Timer3\n");
	STC_REG->timer3_ctl = TIMER3_CONFIG_STC | TIMER3_RELOAD | TIMER3_RUN;
	STC_REG->timer3_pres_val = 0;
	STC_REG->timer3_reload = TIMER3_TICKS;
	STC_REG->timer3_cnt = TIMER3_TICKS;
	hal_interrupt_configure(154, 0, 1);
}


void timer3_callback(void)
{
	hal_interrupt_acknowledge(154);
	printf("@Hello[%d]\n", g_repeat_cnt);
	g_repeat_cnt++;
}


void timer_test_init()
{
	static interrupt_operation timer3_opt;

	memcpy(timer3_opt.dev_name, "Timer3", strlen("Timer3"));
	timer3_opt.vector = 154;
	timer3_opt.device_config = timer3_isr_cfg;
	timer3_opt.interrupt_handler = timer3_callback;
	interrupt_register(&timer3_opt);

}


void timer_test()
{
	printf("Timer test start!\n");

	g_repeat_cnt = 0;

	STC_REG->timer3_ctl = TIMER3_CONFIG_STC | TIMER3_RELOAD | TIMER3_RUN;
	STC_REG->timer3_pres_val = 0;
	STC_REG->timer3_reload = TIMER3_TICKS - 1;
	STC_REG->timer3_cnt = TIMER3_TICKS - 1;

	timer3_interrupt_control_mask(1);

	while(g_repeat_cnt < 6);

	timer3_interrupt_control_mask(0);

	printf("Timer3 interrupt test finished\n");

}