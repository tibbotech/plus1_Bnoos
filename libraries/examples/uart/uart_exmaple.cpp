#include "Arduino.h"
#include "HardwareSerial.h"

void setup()
{
	printf("Build @%s, %s\n", __DATE__, __TIME__);
	Serial2.begin(115200);

	Serial2.print("abcdef123456798abdefdefg");
	Serial2.print("11111");

	printf("get = %c  \n",Serial1.read());
}

void loop()
{
	
}
