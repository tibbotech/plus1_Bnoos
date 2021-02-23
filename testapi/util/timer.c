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

#define TIMER3_TICKS		(90 - 1)		/* 1s */
#define TIMER2_TICKS		(90 - 1)		/* 1s */

#define TIMER_CONFIG_STC	(1 << 2)	/* src: stc */
#define TIMER_RELOAD		(1 << 1)	/* timer auto reload */
#define TIMER_RUN 			(1 << 0)	/* timer run */
#define TIMER_STOP			(0 << 0)	/* timer stop */

#define TIMER_1MS_PRES_VALUE	(9)		// 1ms=1.1us*(TIMER_1MS_PRES_VALUE+1)*(TIMER2_TICKS+1)

#define TIMER3_INT  (154)
#define TIMER2_INT  (153)

static volatile unsigned int g_repeat_cnt = 0;

void timer3_callback(int vector)
{
	printf("@Hello[%d]\n", ++g_repeat_cnt);
}

extern void qch_timer_callback(int);

void timer_test_init()
{
#ifdef QCH_TEST
	isr_t isr = qch_timer_callback;
#else
	isr_t isr = timer3_callback;
#endif
	STC_REG->timer3_ctl = TIMER_CONFIG_STC | TIMER_RELOAD;
	STC_REG->timer3_pres_val = 999;
	STC_REG->timer3_reload = TIMER3_TICKS;
	STC_REG->timer3_cnt = TIMER3_TICKS;
	interrupt_register(TIMER3_INT, "TIMER3", isr, 0);
}

void timer_test()
{
	printf("Timer test start!\n");

	g_repeat_cnt = 0;

	STC_REG->timer3_ctl |= TIMER_RUN;

	while (g_repeat_cnt < 6);

	STC_REG->timer3_ctl &= ~TIMER_RUN;

	printf("Timer3 interrupt test finished\n");

}


void SP_start_timer2(void (*timer_callback)(int))
{
	isr_t isr = timer_callback;

	STC_REG->timer2_ctl = TIMER_CONFIG_STC | TIMER_RELOAD;
	STC_REG->timer2_pres_val = TIMER_1MS_PRES_VALUE;  
	STC_REG->timer2_reload = TIMER2_TICKS;
	STC_REG->timer2_cnt = TIMER2_TICKS;
	
	interrupt_register(TIMER2_INT, "TIMER2", isr, 0);

	STC_REG->timer2_ctl |= TIMER_RUN;
}

void SP_start_timer3(void (*timer_callback)(int))
{
    isr_t isr = timer_callback;

    STC_REG->timer3_ctl = TIMER_CONFIG_STC | TIMER_RELOAD;
    STC_REG->timer3_pres_val = TIMER_1MS_PRES_VALUE*74;
    STC_REG->timer3_reload = TIMER3_TICKS;
    STC_REG->timer3_cnt = TIMER3_TICKS;

    interrupt_register(TIMER3_INT, "TIMER3", isr, 0);

    STC_REG->timer3_ctl |= TIMER_RUN;
}
