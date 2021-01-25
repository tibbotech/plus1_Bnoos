#include <types.h>
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
char linebuf[300];
void * _sbrk(int incr)
{
	static void *heapP = &__bss_end__;
	void *ret = heapP;
	heapP += incr;
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
#endif
