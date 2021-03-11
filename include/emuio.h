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
EXTERN int psprintf(char *dest, const char *format, ...);
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
#define UART_LSR_PE 		(1<<2)
#define UART_LSR_OE 		(1<<3)
#define UART_LSR_FE			(1<<4)

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

static char uart1buf[300];

#define db_printf(s...) \
	do { psprintf(uart1buf, ## s); UART1_puts(uart1buf); } while (0)

#define UART1_LSR_TX_RDY     (1 << 0)
#define UART1_LSR_RX_RDY     (1 << 1)
#define UART1_TX_EMPTY       (1 << 6)
#define UART1_RTS_B			(1 << 1)

	// UART
//#define UART1_REG            UART1_REG
#define UART1_tx_rdy()       (UART1_REG->lsr & UART1_LSR_TX_RDY)
#define UART1_rx_rdy()       (UART1_REG->lsr & UART_LSR_RX_RDY)
#define UART1_tx_empty()     (UART1_REG->lsr & UART_TX_EMPTY)
#define UART1_putc_nw(c)     (UART1_REG->dr = (c))
#define UART1_getc()         (UART1_REG->dr)

#define UART1_wait()         do { while (!UART1_tx_rdy()) ;} while (0)
#define UART1_flush()        do { while (!UART1_tx_empty()) ;} while (0)
#define UART1_putc(c)        do { UART1_wait(); UART1_putc_nw(c); } while (0)
#define UART1_putc_nl(c)     do { char _c=(c); UART1_putc(_c); if (_c == 0x0a) UART1_putc(0x0d);} while (0)
#define UART1_puts(s) \
	do { \
		int __c; \
		const char *__s = (const char *)(s); \
		while ((__c = *__s++)) { \
			UART1_putc_nl(__c); \
		} \
	} while (0)


#define XTAL_CLK               (27 * 1000 * 1000)
#define UART_SRC_CLK        (XTAL_CLK)
		/*
		 * X = ((sclk + baud/2) / baud)
		 * DIV_H = X  >> 12
		 * DIV_L = (X & 0xf) << 12 | ((X >> 4) & 0xff - 1)
		 */
#define UART_BAUD_DIV_H(baud, sclk)     ((((sclk) + ((baud) / 2)) / (baud)) >> 12)
#define UART_BAUD_DIV_L(baud, sclk)     ((((((sclk) + ((baud) / 2)) / (baud)) & 0xf) << 12) | \
							 ((((((sclk) + ((baud) / 2)) / (baud)) >> 4) & 0xff) - 1))
#define BAUDRATE            115200

#endif /* _EMUIO_H_ */
