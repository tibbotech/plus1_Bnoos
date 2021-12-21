/*
 * Copyright (c) 2018 Xilinx, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**************************************************************************
 * FILE NAME
 *
 *       a926_rproc.c
 *
 * DESCRIPTION
 *
 *       This file define Xilinx Zynq A9 platform specific remoteproc
 *       implementation.
 *
 **************************************************************************/

#include <openamp/remoteproc.h>
#include <metal/atomic.h>
#include <metal/device.h>
#include <metal/irq.h>
#include "platform_info.h"
#include "core_armv5.h"

static int a926_proc_irq_handler(int vect_id, void *data)
{
	struct remoteproc *rproc = data;
	struct remoteproc_priv *prproc;

	if (MBOX_NOTIFICATION == 0xDEADC0DE) {
		MBOX_NOTIFICATION = 0;
		printf("!!!!!! a926 reset !!!!!!\n");
		MMU_Disable();
		IRQ_Clear(vect_id);
		__asm volatile (
			"LDR	R0, =0xffff00c0	\n\t"
			"STMFD	SP!, {R0}		\n\t"	/* Push R0. */
			"LDMFD	SP!, {PC}^		"		/* Pop PC.  */
		);
	}

	(void)vect_id;
	if (!rproc)
		return METAL_IRQ_NOT_HANDLED;
	prproc = rproc->priv;
	atomic_flag_clear(&prproc->nokick);
	return METAL_IRQ_HANDLED;
}

static struct remoteproc *
a926_proc_init(struct remoteproc *rproc,
			struct remoteproc_ops *ops, void *arg)
{
	struct remoteproc_priv *prproc = arg;
	unsigned int irq_vect;
	int ret;

	if (!rproc || !prproc || !ops)
		return NULL;
	rproc->priv = prproc;
	rproc->ops = ops;
	atomic_flag_test_and_set(&prproc->nokick);

	/* Register interrupt handler and enable interrupt */
	irq_vect = IRQ_NOTIFICATION;
	metal_irq_register(irq_vect, a926_proc_irq_handler, rproc);
	metal_irq_enable(irq_vect);
	xil_printf("Successfully intialize remoteproc.\r\n");
	return rproc;
}

static void *
a926_proc_mmap(struct remoteproc *rproc, metal_phys_addr_t *pa,
			metal_phys_addr_t *da, size_t size,
			unsigned int attribute, struct metal_io_region **io)
{
	struct remoteproc_mem *mem;
	metal_phys_addr_t lpa, lda;
	struct metal_io_region *tmpio;

	lpa = *pa;
	lda = *da;

	if (lpa == METAL_BAD_PHYS && lda == METAL_BAD_PHYS)
		return NULL;
	if (lpa == METAL_BAD_PHYS)
		lpa = lda;
	if (lda == METAL_BAD_PHYS)
		lda = lpa;

	if (!attribute)
		attribute = NORM_NONCACHE | STRONG_ORDERED;
	mem = metal_allocate_memory(sizeof(*mem));
	if (!mem)
		return NULL;
	tmpio = metal_allocate_memory(sizeof(*tmpio));
	if (!tmpio) {
		metal_free_memory(mem);
		return NULL;
	}
	remoteproc_init_mem(mem, NULL, lpa, lda, size, tmpio);
	/* va is the same as pa in this platform */
	metal_io_init(tmpio, (void *)lpa, &mem->pa, size,
		      sizeof(metal_phys_addr_t)<<3, attribute, NULL);
	remoteproc_add_mem(rproc, mem);
	*pa = lpa;
	*da = lda;
	if (io)
		*io = tmpio;
	return metal_io_phys_to_virt(tmpio, mem->pa);
}

static int a926_proc_notify(struct remoteproc *rproc, uint32_t id)
{
	struct remoteproc_priv *prproc;

	if (!rproc)
		return -1;
	prproc = rproc->priv;

	/* Trigger MBOX */
	MBOX_TO_NOTIFY = id;

	return 0;
}

/* processor operations from r5 to a53. It defines
 * notification operation and remote processor managementi operations. */
struct remoteproc_ops a926_proc_ops = {
	.init = a926_proc_init,
	.remove = NULL,
	.mmap = a926_proc_mmap,
	.notify = a926_proc_notify,
	.start = NULL,
	.stop = NULL,
	.shutdown = NULL,
};
