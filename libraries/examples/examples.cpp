#include "Arduino.h"
#include "VirtIOSerial.h"
#include "HardwareSerial.h"

void setup()
{
	SerialVirtIO.begin();
	Serial2.begin(115200);

	printf("Build @%s, %s\n", __DATE__, __TIME__);//UART0 output
	Serial.print("Build @%s, %s\n", __DATE__, __TIME__);//VirtIO output
	Serial2.print("Build @%s, %s\n", __DATE__, __TIME__);//UART2 output

}

void loop()
{
	if (SerialVirtIO.available()) {
		String s = SerialVirtIO.readString();
		SerialVirtIO.print(s); // write back
	}
}
