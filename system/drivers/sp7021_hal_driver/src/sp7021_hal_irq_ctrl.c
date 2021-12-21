#include "sp7021_hal_irq_ctrl.h"

#ifndef IRQ_GIC_LINE_COUNT
#define IRQ_GIC_LINE_COUNT      (200U)
#endif

static IRQHandler_t IRQTable[IRQ_GIC_LINE_COUNT] = { 0U };
static uint32_t	 IRQ_ID0 = 0;

__STATIC_INLINE void __reset_irq()
{
	int i = 0;
	for (i = 0; i < 7; i++)
	{
		SP_IRQ_CTRL->type[i] = ~0;
		SP_IRQ_CTRL->polarity[i] = 0;
		SP_IRQ_CTRL->priority[i] = ~0;
		SP_IRQ_CTRL->mask[i] = 0;
	}
}

__STATIC_INLINE void GIC_EnableIRQ(IRQn_Type IRQn)
{
	SP_IRQ_CTRL->mask[IRQn/32U] |= (1 << (IRQn%32U));
}

__STATIC_INLINE void GIC_DisableIRQ(IRQn_Type IRQn)
{
	SP_IRQ_CTRL->mask[IRQn/32U] &= ~(1 << (IRQn%32U));
}

__STATIC_INLINE uint32_t GIC_GetEnableIRQ(IRQn_Type IRQn)
{
	return (SP_IRQ_CTRL->mask[IRQn / 32U] >> (IRQn % 32U)) & 1UL;
}

__STATIC_INLINE void GIC_SetIRQType(IRQn_Type IRQn, uint32_t type, uint32_t polarity)
{
	if (type)
	{
		SP_IRQ_CTRL->type[IRQn / 32U] |= (1 << IRQn%32U);
	}
	else
	{
		SP_IRQ_CTRL->type[IRQn / 32U] &= ~(1 << IRQn%32U);
	}
	if (polarity)
	{
		SP_IRQ_CTRL->polarity[IRQn / 32U] |= (1 << IRQn%32U);
	}
	else
	{
		SP_IRQ_CTRL->polarity[IRQn / 32U] &= ~(1 << IRQn%32U);
	}
}

__STATIC_INLINE uint32_t GIC_GetIRQType(IRQn_Type IRQn)
{
	return (SP_IRQ_CTRL->type[IRQn / 32U] >> (IRQn % 32U)) & 1UL;
}

__STATIC_INLINE uint32_t GIC_GetIRQPolarity(IRQn_Type IRQn)
{
	return (SP_IRQ_CTRL->polarity[IRQn / 32U] >> (IRQn % 32U)) & 1UL;
}

__STATIC_INLINE uint32_t GIC_SetIRQPirority(IRQn_Type IRQn, uint32_t priority)
{
	if(priority == 1)
	{
		SP_IRQ_CTRL->priority[IRQn/32U] |= 1 << (IRQn%32U);
	}
	else
	{
		SP_IRQ_CTRL->priority[IRQn/32U] &= ~(1 << IRQn%32U);
	}
}

__STATIC_INLINE uint32_t GIC_GetIRQPirority(IRQn_Type IRQn)
{
	return (SP_IRQ_CTRL->priority[IRQn / 32U] >> (IRQn % 32U)) & 1UL;
}

__STATIC_INLINE void GIC_ClearIRQ(IRQn_Type IRQn)
{
	SP_IRQ_CTRL->clear[IRQn/32U] = (1 << (IRQn%32U));
}

__STATIC_INLINE uint32_t GIC_GetActiveIRQ(void)
{
	uint32_t i=0, j = 0;
	uint32_t irqn;
	for (j = 0; j < 7; j++)
	{
		if ((SP_IRQ_CTRL->group >> (8 + j)) & 1UL)
			break;
	}

	for (i = 0; i < 32; i++)
	{
		if ((SP_IRQ_CTRL->masked_irq[j] >> i) & 1UL)
			break;
	}
	irqn = j * 32 + i;

	return irqn;
}

__STATIC_INLINE uint32_t GIC_GetActiveFIQ(void)
{
	uint32_t i=0, j = 0;
	uint32_t irqn;
	for (j = 0; j < 7; j++)
	{
		if ((SP_IRQ_CTRL->group >> j) & 1UL)
			break;
	}

	for (i = 0; i < 32; i++)
	{
		if ((SP_IRQ_CTRL->masked_fiq[j] >> i) & 1UL)
			break;
	}
	irqn = j * 32 + i;

	return irqn;
}



int32_t IRQ_Initialize (void)
{

	uint32_t i = 0;

	for (i=0; i<IRQ_GIC_LINE_COUNT; i++ )
	{
		IRQTable[i] = (IRQHandler_t)NULL;
	}
	__disable_irq();
	__reset_irq();
	__enable_irq();

	return 0;
}

int32_t IRQ_Unitialize(void)
{
	__disable_irq();
	__reset_irq();

}


int32_t IRQ_SetHandler (IRQn_ID_t irqn, IRQHandler_t handler)
{
	int32_t status = 0;
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT))
	{
		IRQTable[irqn] = handler;
		status =  0;
	}
	else
	{
		status = -1;
	}

	return status;
}

IRQHandler_t IRQ_GetHandler (IRQn_ID_t irqn)
{
	IRQHandler_t h;
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT))
	{
		h = IRQTable[irqn];
	}
	else
	{
		h = (IRQHandler_t)0;
	}

	return h;
}
 
int32_t IRQ_Enable (IRQn_ID_t irqn)
{
	int32_t status;
	if ((irqn >=0) && (irqn<(IRQn_ID_t)IRQ_GIC_LINE_COUNT))
	{
		GIC_EnableIRQ ((IRQn_Type)irqn);
		status = 0;
	}
	else
	{
		status = -1;
	}

	return status;
}

int32_t IRQ_Disable (IRQn_ID_t irqn)
{
	int32_t status;
	if ((irqn >=0) && (irqn<(IRQn_ID_t)IRQ_GIC_LINE_COUNT))
	{
		GIC_DisableIRQ ((IRQn_Type)irqn);
		status = 0;
	}
	else
	{
		status = -1;
	}

	return status;
}

uint32_t IRQ_GetEnableState (IRQn_ID_t irqn)
{
	uint32_t enable;
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT))
	{
		enable = GIC_GetEnableIRQ((IRQn_Type)irqn);
	}
	else
	{
		enable = -1;
	}

	return enable;
}

int32_t IRQ_SetMode (IRQn_ID_t irqn, uint32_t mode)
{
	int32_t status = 0;
	int32_t type, polarity;

	mode = mode & 0xF;	//bits[0:3] is really vaild, bits[4:7] used for fix exti bug
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT))
	{
		switch (mode)
		{
			case IRQ_MODE_TRIG_LEVEL_HIGH:type = 0; polarity = 0; break;
			case IRQ_MODE_TRIG_LEVEL_LOW:type = 0; polarity = 1; break;
			case IRQ_MODE_TRIG_EDGE_RISING:type = 1; polarity = 0; break;
			case IRQ_MODE_TRIG_EDGE_FALLING:type = 1; polarity = 1; break;
			default:status = -1;break;
		}
		GIC_SetIRQType(irqn, type, polarity);
		status = 0;
	}
	else
	{
		status = -1;
	}

	return status;
}

uint32_t IRQ_GetMode (IRQn_ID_t irqn)
{
	uint32_t val;
	uint32_t mode;
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT))
	{
		val = GIC_GetIRQType(irqn);
		if (val == 1U)
		{
			mode = IRQ_MODE_TRIG_EDGE_RISING;
			return mode;
		}

		val = GIC_GetIRQPolarity(irqn);
		if (val == 0)
		{
			mode = IRQ_MODE_TRIG_LEVEL_HIGH;
		}
		else
		{
			mode = IRQ_MODE_TRIG_LEVEL_LOW;
		}
	}

	return mode;
}

int32_t IRQ_SetPriority (IRQn_ID_t irqn, uint32_t priority)
{
	int32_t status;
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT))
	{
		GIC_SetIRQPirority(irqn, priority);
		status = 0;
	}
	else
	{
		status = -1;
	}

	return status;
}

uint32_t IRQ_GetPriority (IRQn_ID_t irqn)
{
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT))
	{
		return GIC_GetIRQPirority(irqn);
	}

	return 0;
}

int32_t IRQ_Clear(IRQn_ID_t irqn)
{
	int32_t status;
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT))
	{
		GIC_ClearIRQ(irqn);
		status = 0;
	}
	else
	{
		status = -1;
	}

	return status;
}


IRQn_ID_t IRQ_GetActiveIRQ (void)
{
	return GIC_GetActiveIRQ();
}

IRQn_ID_t IRQ_GetActiveFIQ (void)
{
	return GIC_GetActiveFIQ();
}

volatile int irq_no  = 0;
void IRQ_HANDLE(void)
{
	ISR_SAVE_CONTEXT();
	IRQHandler_t handler = NULL;
	IRQn_ID_t irqn = 0;
	irqn = GIC_GetActiveIRQ();
	handler = IRQ_GetHandler(irqn);
	irq_no = irqn;
	if (handler != NULL)
		handler();
	IRQ_Clear(irqn);
	ISR_RESTORE_CONTEXT();
}

void FIQ_HANDLE(void)
{
	ISR_SAVE_CONTEXT();
	IRQHandler_t handler = NULL;
	IRQn_ID_t irqn = 0;
	irqn = GIC_GetActiveFIQ();
	handler = IRQ_GetHandler(irqn);
	if (handler != NULL)
		handler();
	IRQ_Clear(irqn);
	ISR_RESTORE_CONTEXT();
}
#if 0
void RESET_HANDLE(void)
{
	printf("%s\n", __FUNCTION__);
	while(1);
	return;
}

void UNDEF_INS_HANDLE( void )
{
	printf("%s\n", __FUNCTION__);
	while(1);
	return;
}

void SWI_HANDLE( void )
{
	printf("%s\n", __FUNCTION__);
	while(1);
    return;
}

void PREFE_ABORT_HANDLE( void )
{
	printf("%s\n", __FUNCTION__);
	while(1);
	return;
}

void DATA_ABORT_HANDLE( void )
{
	printf("%s\n", __FUNCTION__);
	while(1);
    return;
}

void NO_USED_HANDLE( void )
{
	printf("%s\n", __FUNCTION__);
	while(1);
    return;
}
#else
#define ARM_MODE_USER			0x10
#define ARM_MODE_FIQ			0x11
#define ARM_MODE_IRQ			0x12
#define ARM_MODE_SVC			0x13
#define ARM_MODE_ABORT			0x17
#define ARM_MODE_UNDEFINED		0x1B
#define ARM_MODE_SYSTEM			0x1F
#define ARM_MODE_MASK			0x1F

extern void *__except_stack_top;

/* Exception Handler */
void do_exec(uint32_t type, uint32_t lr)
{
    char *str = "?";
    int i;
    uint32_t *bptr = ((uint32_t *)&__except_stack_top) - 13;

    switch (type & ARM_MODE_MASK) {
    case ARM_MODE_ABORT: str = "ABORT"; break;
    case ARM_MODE_FIQ: str = "FIQ"; break;
    case ARM_MODE_IRQ: str = "IRQ"; break;
    case ARM_MODE_SVC: str = "SVC"; break;
    case ARM_MODE_SYSTEM: str = "System"; break;
    case ARM_MODE_UNDEFINED: str = "Undef"; break;
    case ARM_MODE_USER: str = "User"; break;
    }
    printf("[%s]\n", str);
	printf("CPSR\t= 0x%x\n", type);
	printf("LR\t= 0x%x\n", lr);
	
    for (i = 0; i < 13; i++) {
        printf("R%d\t= 0x%x\n", i, bptr[i]);
    }
    while (1);
}
#endif
