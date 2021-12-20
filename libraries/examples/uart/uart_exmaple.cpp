#include "Arduino.h"
#include "HardwareSerial.h"

void setup()
{
	Serial2.begin(115200);
	
	Serial2.print("abcdef123456798abdefdefg");
	Serial2.print("9999");

	printf("get = %c  \n",Serial2.read());
}

void loop()
{

}
