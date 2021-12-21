/**
  * \file syscall.c
  *
  * Implementation of newlib syscall.
  *
  */

#include "sunplus_def.h"

#if defined (  __GNUC__  ) /* GCC CS3 */
#include <sys/stat.h>
#endif

#include <errno.h>
#undef errno
extern int errno;

#ifdef UNUSED
  #undef UNUSED
#endif
#ifdef __GNUC__
  #define UNUSED(x) x ## _UNUSED __attribute__((__unused__))
#else
  #define UNUSED(x) x ## _UNUSED
#endif



#define UART_LSR_TX_RDY     (1 << 0)
#define UART_LSR_RX_RDY     (1 << 1)
#define UART_TX_EMPTY       (1 << 6)
#define UART_RTS_B                      (1 << 1)

// UART
#define UART_REG            SP_UART0
#define UART_tx_rdy()       (SP_UART0->lsr & UART_LSR_TX_RDY)
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



/*This funciton for new libc atomic operation for arm926(ARMv5) */
void __sync_synchronize(void)
{

}

int __atomic_compare_exchange_1(char *obj, char *expected, char desired)
{
        int ret = (*obj == *expected);
        if (ret)
                *obj = desired;
        else
                *expected = *obj;
        return ret;
}
int __atomic_compare_exchange_4(int *obj, int *expected, int desired)
{
    int ret = (*obj == *expected);
    if (ret)
            *obj = desired;
    else
            *expected = *obj;
    return ret;
}

 __attribute__((weak)) caddr_t  _sbrk(int incr)
{
	extern char __bss_end__;
	static char *__brkval = &__bss_end__;
    char *ret = __brkval;
	
     __brkval += incr;
    return (caddr_t)ret;
}


 __attribute__((weak))
 int _close(UNUSED(int file))
 {
   return -1;
 }
 
 __attribute__((weak))
 int _fstat(UNUSED(int file), struct stat *st)
 {
   st->st_mode = S_IFCHR ;
   return 0;
 }

 __attribute__((weak))
 int _isatty(UNUSED(int file))
 {
	return 1;
 }
 
 __attribute__((weak))
 int _lseek(UNUSED(int file), UNUSED(int ptr), UNUSED(int dir))
 {
	return 0;
 }
 
 __attribute__((weak))
 int _read(UNUSED(int file), UNUSED(char *ptr), UNUSED(int len))
 {
 	return 0;
 }


int __write(int fd, const char *buf, int count)
{
	 int ret = count;
	 while (count--) UART_putc_nl(*(char *)(buf++));
	//ret = uart_debug_write(buf, count);
	 return ret;
}

__attribute__((weak))
void _exit(UNUSED(int status))
{
	for (;;) ;
}

__attribute__((weak))
int _kill(UNUSED(int pid), UNUSED(int sig))
{
  	errno = EINVAL;
  	return -1;
}

__attribute__((weak))
int _getpid(void)
{
  	return 1;
}

