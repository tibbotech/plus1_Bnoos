#include <types.h>
#include "common_all.h"
#include "sp_interrupt.h"

void _RESET_handler( void ) __attribute__ ((naked));
void _UNDEF_INS_handler( void ) __attribute__ ((naked));
void _SWI_handler( void ) __attribute__ ((naked));
void _PREFE_ABORT_handler( void ) __attribute__ ((naked));
void _DATA_ABORT_handler( void ) __attribute__ ((naked));
void _NO_USED_handler( void ) __attribute__ ((naked));
void _IRQ_handler( void ) __attribute__ ((naked));
void _FIRQ_handler( void ) __attribute__ ((naked));

interrupt_operation  *int_opt_table[32 * 7] = {0};


static void interrupt_reset(void)
{
    int i = 0;


    /*Interrupts Initialize */
    for (i = 0; i < 7; i++) {
        // all edge
        HAL_WRITE_UINT32(SPINTC_TYPE + (i * 4), ~0);
        // all high-active
        HAL_WRITE_UINT32(SPINTC_POLAR + (i * 4), 0);
        // all irq
        HAL_WRITE_UINT32(SPINTC_PRIO + (i * 4), ~0);
        // all mask
        HAL_WRITE_UINT32(SPINTC_MASK + (i * 4), 0);
    }
}

static void interrupt_enable(void)
{
    portENABLE_INTERRUPTS();
}

static void interrupt_disable(void)
{
    portDISABLE_INTERRUPTS();
}

static int check_int_opt(interrupt_operation *int_opt)
{
    if (int_opt->vector < 32 * 7) {
        printf("check_int_opt: [vector] pass \n");
        if (int_opt->device_config != 0 && ((int_opt->interrupt_handler != 0) || (int_opt->interrupt_handler_with_vector != 0))) {
            printf("check_int_opt: [callback] pass \n");
            return 0;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

static unsigned lookup_vector(void)
{
    unsigned dwMask;
    unsigned vector = 0, i;

    // get #
    for (i = 0; i < 7; i++) {
        // irq
        HAL_READ_UINT32(SPINTC_MSKIRQ + (i * 4), dwMask);
        if (dwMask) {
            vector = (i * 32);
            // find the bit
            while (!(dwMask & 0x1)) {
                dwMask >>= 1;
                vector++;
            }
            break;
        }

        // fiq
        HAL_READ_UINT32(SPINTC_MSKFIQ + (i * 4), dwMask);
        if (dwMask) {
            vector = (i * 32);
            // find the bit
            while (!(dwMask & 0x1)) {
                dwMask >>= 1;
                vector++;
            }
            break;
        }
    }

    return vector;
}

static void excute_int_config_opt(void)
{
    size_t i = 0;
    prn_string("Enter excute_int_config_opt() \n");
    while (i < 32 * 7) {
        if (int_opt_table[i] != 0) {
            (int_opt_table[i])->device_config();
        }
        i++;
    }
    prn_string("Exit excute_int_config_opt() \n");
}

static void excute_int_handler(unsigned vector)
{
    if (vector < 32 * 7) {
        if (int_opt_table[vector] != 0) {
            if ((int_opt_table[vector])->interrupt_handler != 0) {
                (int_opt_table[vector])->interrupt_handler();
            }
            if (int_opt_table[vector]->interrupt_handler_with_vector != 0) {
                int_opt_table[vector]->interrupt_handler_with_vector(vector);
            }
        } else {
            hal_interrupt_acknowledge(vector);  //unhandled int ack
        }
    }
}

/* invoked by interrupt manager module */
void sp_interrupt_setup()
{
    prn_string("Enter sp_interrupt_setup() \n");

    interrupt_disable();
    prn_string("intr reset\n");
    interrupt_reset();

    prn_string("reset done.");
    /* interrupt configuration registed by driver invoked here */
    excute_int_config_opt();

    prn_string("config opt done.");
    /* enable interrupt */
    interrupt_enable();

    prn_string("Exit sp_interrupt_setup() \n");
}

/* invoked by interrupt manager module */
void sp_interrupt_disable()
{
    interrupt_disable();
}

/* invoked by interrupt driver */
int interrupt_register(interrupt_operation *int_opt)
{
    if (check_int_opt(int_opt) < 0) {
        return -1;
    }

    int_opt_table[int_opt->vector] = int_opt;

    // prn_string("interrupt_register successfully \n");
    return 0;
}

/* invoked by interrupt driver */
int interrupt_unregister(interrupt_operation *int_opt)
{
    if (check_int_opt(int_opt) < 0) {
        return -1;
    }
    int_opt_table[int_opt->vector] = 0;
    // prn_string("interrupt_unregister successfully \n");
    return 0;
}

void hal_interrupt_configure(int vector, int level, int up)
{
    unsigned int dwRegValue;

    // 0:level-type, 1: edge-type
    HAL_READ_UINT32(SPINTC_TYPE + ((vector / 32) * 4), dwRegValue);
    if (level)
    {
        dwRegValue &= ~(1 << (vector % 32));
    }
    else
    {
        dwRegValue |= 1 << (vector % 32);
    }
    HAL_WRITE_UINT32(SPINTC_TYPE + ((vector / 32) * 4), dwRegValue);

    // 0:high-active, 1: low-active
    HAL_READ_UINT32(SPINTC_POLAR + ((vector / 32) * 4), dwRegValue);
    if (up)
    {
        dwRegValue &= ~(1 << (vector % 32));
    }
    else
    {
        dwRegValue |= 1 << (vector % 32);
    }
    HAL_WRITE_UINT32(SPINTC_POLAR + ((vector / 32) * 4), dwRegValue);
}

void hal_interrupt_unmask(int vector)
{
    unsigned int dwMask;

    // mask # pass
    HAL_READ_UINT32(SPINTC_MASK + ((vector / 32) * 4), dwMask);
    dwMask |= 0x00000001 << (vector % 32);
    HAL_WRITE_UINT32(SPINTC_MASK + ((vector / 32) * 4), dwMask);
}

void hal_interrupt_mask(int vector)
{
    unsigned int dwMask;

    // mask # unpass
    HAL_READ_UINT32(SPINTC_MASK + ((vector / 32) * 4), dwMask);
    dwMask &= ~(0x00000001 << (vector % 32));
    HAL_WRITE_UINT32(SPINTC_MASK + ((vector / 32) * 4), dwMask);
}

void hal_interrupt_acknowledge(int vector)
{
    unsigned int dwMask;

    dwMask = 0x00000001 << (vector % 32);
    // clear #
    HAL_WRITE_UINT32(SPINTC_CLR + ((vector / 32) * 4), dwMask);
}

/* invoked by interrupt vector */
void _RESET_handler( void )
{
    prn_string("_RESET_handler()\n");
}

void _UNDEF_INS_handler( void )
{
    prn_string("_UNDEF_INS_handler()\n");
}

void _SWI_handler( void )
{
    prn_string("_SWI_handler()\n");
}

void _PREFE_ABORT_handler( void )
{
    prn_string("_PREFE_ABORT_handler()\n");
}

void _DATA_ABORT_handler( void )
{
    prn_string("_DATA_ABORT_handler()\n");
}

void _NO_USED_handler( void )
{
    prn_string("_NO_USED_handler()\n");
}

void _IRQ_handler( void )
{
    isrSAVE_CONTEXT();

    unsigned vector = lookup_vector();
    excute_int_handler(vector);
    isrRESTORE_CONTEXT();
}

void _FIRQ_handler( void )
{
    prn_string("_FIRQ_handler()\n");
}
