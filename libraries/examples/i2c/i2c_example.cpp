/*
 * If you want to test this file. Please add this file in Makefile(lines. 110)
 *
 * Test i2c on a temperature sensor(SHT3X).
 * The transmission of Arduino layer(twi.c) is implemented as burst_irq transmit.
 * if want to use other mode.Please look over HAL layer(sp7021_hal_i2c.c).
 * And replace function in twi.c(). 
 * e.x.
 * 		i2c_master_write()
 * 		{
 * 			HAL_I2C_Master_Transmit_IT()  ---> new tramsmit mode
 * 		}
 * similarly,
 * 		i2c_master_read()
 *		{
 * 			HAL_I2C_Master_Receive_IT()   ---> new receive mode
 *		}
 */
#include "Arduino.h"
#include "Wire.h"

#define SLAVE_ADDR 0x44

static uint8_t tx_buff[32];
static uint8_t rx_buff[32];

void setup()
{
}

void loop()
{
	int i = 0;

	delay(3000);

	Wire.begin();

	tx_buff[0] = 0x24;
	tx_buff[1] = 0x00;

	Wire.beginTransmission(SLAVE_ADDR);
	Wire.write(tx_buff, 2);
	Wire.endTransmission();
	Wire.requestFrom(SLAVE_ADDR, 6);

	while(Wire.available())
	{
		rx_buff[i] = Wire.read();
		i++;
	}

	int temp = (rx_buff[0]<<8) | rx_buff[1];
	temp = temp * 17500/65536;
	int temp_L = temp%100;
	int temp_H = temp/100 -45;
	
	int RH = (rx_buff[3] << 8) | rx_buff[4];
	RH = RH * 10000/65536;
	int RH_L = RH % 100;
	int RH_H = RH / 100;
	::printf("\n temp=%d.%d   %d.%d    \n", temp_H, temp_L, RH_H, RH_L);
	
}
