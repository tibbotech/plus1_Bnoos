/* This is a sample demonstration application that showcases usage of VirtIOSerial
This application is meant to run on the remote CPU running baremetal code.
This application echoes back data that was sent to it by the master core. */

#if defined (VIRTIOCON)

#include "VirtIOSerial.h"

#ifdef __cplusplus
extern "C" {
#endif

int openamp_for_cpp_test_main(int argc, char *argv[])
{
	int ret;
	VirtIOSerial uart;

	while (1) {
#if 0 // method 1
		static char s[256];
		ret = uart.readBytes(s, sizeof(s));
		if (ret > 0) {
			uart.write((const uint8_t *)s, ret); // write back
		}
#else // method 2
		if (uart.available()) {
			String s = uart.readString();
			if (s == "END\n") {
				uart.print("=== OpenAMP VirtIOSerial Test Done!!! ===\n");
				break;
			}
			uart.print(s); // write back
		}
#endif
	}

	return ret;
}

#ifdef __cplusplus
}
#endif

#endif
