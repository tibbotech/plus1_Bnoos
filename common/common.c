#include <common_all.h>

#if 0
int memcmp(UINT8 *s1, UINT8 *s2, int n)
{
	int ret = 0;
	UINT8 *s1_end = s1 + n;

	while(s1 != s1_end) {
		if (*s1++ != *s2++)
			ret = 1;
	}

	return ret;
}

void *memset(UINT8 *s1, int c, int n)
{
	UINT8 *s1_end = s1 + n;

	while (s1 != s1_end)
		*s1++ = c;

	return s1;
}

void *memcpy(UINT8 *s1, const UINT8 *s2, int n)
{
	UINT8 *s1_end = s1 + n;

	while(s1 != s1_end)
		*s1++ = *s2++;

	return s1;
}

int strlen(const char * str)
{
	const char *start = str;
	while (*str) str++;
	return str - start;
}
#else
extern char __bss_end__;
void *__brkval = &__bss_end__;
void * _sbrk(int incr)
{
	void *ret = __brkval;
	__brkval += incr;
	return ret;
}

void abort(void)
{
	while (1);
}

void _exit(int c)
{
	abort();
}

void _kill(int pid, int signal)
{
}

int _getpid(void)
{
	return 0;
}

#ifdef USE_PSPRINTF
char linebuf[300];
#else
int _write(int fd, const void *buf, int count)
{
	int ret = count;
	while (count--) UART_putc_nl(*(char *)(buf++));
	return ret;
}

int _read(int fd, void *buf, int count)
{
	return -1;
}

int _close(int fd)
{
	return 0;
}

int _open(const char *filename, int flags)
{
	return 0;
}

int _isatty(int fd)
{
	return 1;
}

int _lseek(int fd, int offset, int whence)
{
	return -1;
}

int _fstat(int fd, struct stat *buf)
{
	return 0;
}
#endif
#endif
