#ifndef _EMUIO_H_
#define _EMUIO_H_

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

#define TRACE(tag) printf("\n[%s] %s: %s(%d)\n", tag, __FILE__, __FUNCTION__, __LINE__)

EXTERN int Marlin_main(void);
EXTERN void uart_isr(int vector);
EXTERN int uart_read(void); // return -1 means no data

//#define USE_PSPRINTF // using psprintf@sio.c replace printf
#ifdef USE_PSPRINTF
extern char linebuf[];
EXTERN int psprintf(char *dest, const char *format, ...);
#define printf(s...) \
	do { psprintf(linebuf, ## s); UART_puts(linebuf); } while (0)
#endif

#define dbg_info()	_dbg_info(__FILE__, __LINE__)
#define dbg()		_dbg_info(__FILE__, __LINE__)
#define dbg_stamp(value) //{ *(volatile unsigned int *)RF_GRP(0, 0) = (unsigned int)(value); }

#define UART_LSR_TX_RDY     (1 << 0)
#define UART_LSR_RX_RDY     (1 << 1)
#define UART_TX_EMPTY       (1 << 6)
#define UART_RTS_B			(1 << 1)

// UART
#define UART_REG            UART1_REG
#define UART_tx_rdy()       (UART_REG->lsr & UART_LSR_TX_RDY)
#define UART_rx_rdy()       (UART_REG->lsr & UART_LSR_RX_RDY)
#define UART_tx_empty()     (UART_REG->lsr & UART_TX_EMPTY)
#define UART_putc_nw(c)     (UART_REG->dr = (c))
#define UART_getc()         (UART_REG->dr)

#define UART_wait()         do { while (!UART_tx_rdy()) ;} while (0)
#define UART_flush()        do { while (!UART_tx_empty()) ;} while (0)
#define UART_putc(c)        do { UART_wait(); UART_putc_nw(c); } while (0)
#define UART_putc_nl(c)     do { char _c=(c); UART_putc(_c); if (_c == 0x0a) UART_putc(0x0d);} while (0)
#define UART_puts(s) \
do { \
	int __c; \
	const char *__s = (const char *)(s); \
	while ((__c = *__s++)) { \
		UART_putc_nl(__c); \
	} \
} while (0)

#endif /* _EMUIO_H_ */
