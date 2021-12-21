#include "Arduino.h"
#include "HardwareSerial.h"

void setup()
{
	printf("Build @%s, %s\n", __DATE__, __TIME__);//UART0 print

	/* UART2 print */
	Serial2.begin(115200);//UART2 init
	Serial2.print("Build @");
	Serial2.println(__DATE__);
	Serial2.println(__TIME__);
	Serial2.print("Nonos boot ok!\n");

	printf("Nonos boot ok!\n");//UART0 print
}

void loop()
{

}
