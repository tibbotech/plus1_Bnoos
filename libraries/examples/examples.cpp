#include "Arduino.h"
#include "HardwareSerial.h"

/*
 * This example only show the most basic usage of print.
 * If you want to test more, you can refer to other examples
 * in libraries/examples.
 */
void setup()
{
	printf("####### [arm926] Build @%s, %s ####### \n\n", __DATE__, __TIME__);//UART0 print

#if 0
	/* UART2 print */
	Serial2.begin(115200);//UART2 init
	Serial2.print("Build @");
	Serial2.println(__DATE__);
	Serial2.println(__TIME__);
	Serial2.print("arm926 boot ok!\n");
#endif
	printf("arm926 boot ok!\n");//UART0 print
}

void loop()
{

}
