/*
 * If you want to test this file. Please add this file in Makefile(lines. 110)
 * 
 * This example test SerialVirtIO.
 * entry linux console: enter command as follow:
 *
 *		cat /dev/ttyRPMSG0 &
 * 		echo 1234 > /dev/ttyRPMSG0'
 *
 * stdout display the information as same as input:
 *
 * 		1234
 *
 */
#include "Arduino.h"
#include "VirtIOSerial.h"

void setup()
{
	SerialVirtIO.begin();
}

void loop()
{
	if (SerialVirtIO.available()) {
		String s = SerialVirtIO.readString();
		SerialVirtIO.print(s); // write back
	}
}
