#include "types.h"
#include "temperature_ads1015.h"
#include "gpio_drv.h"
#include "pins.h"




#define I2C_NUM		0  /* contain 4 I2C,from 0--3 */

unsigned short ADS1015_CONFIG;//定义ADS1015配置变量


#define GPIO_PIN_INIT(gpio_index)		GPIO_F_SET(gpio_index,1); \
										GPIO_M_SET(gpio_index,1);\
										GPIO_E_SET(gpio_index,1);\
										GPIO_O_SET(gpio_index,1)

#define I2C_SDA_1()						GPIO_O_SET(TEMP_I2C_SDA,1)
#define I2C_SDA_0()						GPIO_O_SET(TEMP_I2C_SDA,0)

#define I2C_SCL_1()						GPIO_O_SET(TEMP_I2C_SCL,1)
#define I2C_SCL_0()						GPIO_O_SET(TEMP_I2C_SCL,0)

#define I2C_SDA_READ()					GPIO_I_GET(TEMP_I2C_SDA)

#define I2C_SDA_IN()					GPIO_E_SET(TEMP_I2C_SDA,0)
#define I2C_SDA_OUT()					GPIO_E_SET(TEMP_I2C_SDA,1)

#define Delay()							STC_delay_us(20)

void i2c_init(void)
{
	GPIO_PIN_INIT(TEMP_I2C_SDA);
	GPIO_PIN_INIT(TEMP_I2C_SCL);
	Delay();
	Delay();
}
void i2c_start(void)
{
	I2C_SDA_1();
	I2C_SCL_1();
	Delay();
	I2C_SDA_0();
	Delay();
	I2C_SCL_0();
	Delay();
}

void i2c_stop(void)
{
	I2C_SDA_0();
	Delay();
	I2C_SCL_1();
	Delay();
	I2C_SDA_1();
	Delay();
}
void i2c_send_nack(void)
{
	I2C_SCL_0();
	Delay();
	I2C_SDA_1();
	Delay();
	I2C_SCL_1();
	Delay();
	I2C_SCL_0();
	Delay();
}

void i2c_send_ack(void)
{
	I2C_SCL_0();
	Delay();
	I2C_SDA_0();
	Delay();
	I2C_SCL_1();
	Delay();
	I2C_SCL_0();
	Delay();
}

int i2c_read_ack(void)
{
	int ret=1;
	int cnt = 300;
	I2C_SDA_IN();
	I2C_SCL_1();
	Delay();
	while( --cnt > 0 && ret){
		ret = I2C_SDA_READ();
		I2C_SCL_0();
		Delay();
		I2C_SDA_OUT();
	}

	//if(cnt == 0)
	//	printf("i2c slave no ack !!!\n");
	return ret;
}
char i2c_read(void)
{
	char data=0;
	int i=8;
	I2C_SDA_IN();
	Delay();
	while (i--)
	{
		data = data<<1;
		I2C_SCL_1();
		if(I2C_SDA_READ())
			data |= 0x01;
		Delay();
		I2C_SCL_0();
		Delay();
	}
	I2C_SDA_OUT();
	Delay();
	I2C_SDA_0();
	Delay();
	return data;
}

int i2c_write(unsigned int i2c_buff)
{
	int i=8;

	while (i--)
	{
		I2C_SCL_0();
		Delay();
		if(i2c_buff & 0x80)
			I2C_SDA_1();
		else
			I2C_SDA_0();
		i2c_buff = i2c_buff<<1;
		I2C_SCL_1();
		Delay();
	}
	I2C_SCL_0();
	Delay();

	return i2c_read_ack();
}

//ADS1015 read 2bytes data from reg
unsigned short ADS1015_ReadOneByte(unsigned char i2cAddress,unsigned short ReadAddr)
{
	unsigned short temp=0;

	i2c_start();
	i2c_write(i2cAddress<<1);
	i2c_write(ReadAddr);
	i2c_stop();

	i2c_start();
	i2c_write((i2cAddress<<1)+1);
	temp=i2c_read();
	temp=temp<<8;
	i2c_send_ack();
	temp+=i2c_read();
	i2c_send_ack();
	i2c_stop();

	return temp;
}

//ADS1015 write data to reg
void ADS1015_WriteOneByte(unsigned char i2cAddress,unsigned short WriteAddr,unsigned short DataToWrite)
{
	i2c_start();
	
	i2c_write(i2cAddress<<1);	
	i2c_write(WriteAddr);
	i2c_write(DataToWrite>>8);	
	i2c_write(DataToWrite&0xff);
	
	i2c_stop();

}


unsigned short ADS1015_Read(unsigned char channel)
{
int temp;

 // Start with default values
  ADS1015_CONFIG  = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
					ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
					ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
					ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
					ADS1015_REG_CONFIG_DR_3300SPS   | // 1600 samples per second (default)
					ADS1015_REG_CONFIG_MODE_CONTIN;   // continue mode (default)
	
    // Set PGA/voltage range
    ADS1015_CONFIG |= ADS1015_REG_CONFIG_PGA_4_096V;
	if(channel<3)
	{
		switch (channel)
		{
			case (0):
				ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
			break;
			case (1):
				ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
			break;
			case (2):
				ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
			break;
			case (3):
				ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
			break;
		}
		  // Set 'start single-conversion' bit
		ADS1015_WriteOneByte(ADS1015_ADDRESS,ADS1015_REG_POINTER_CONFIG, ADS1015_CONFIG);

		temp=ADS1015_ReadOneByte(ADS1015_ADDRESS,ADS1015_REG_POINTER_CONVERT);
		temp=temp>>4;
		//printf("read data = %d ",temp);
		return temp;
	}
return 0;
}

void ADS1015_Config(unsigned char i2cAddress,unsigned char channel)
{
// Start with default values
ADS1015_CONFIG  = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
				ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
				ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
				ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
				ADS1015_REG_CONFIG_DR_3300SPS   | // 1600 samples per second (default)
				ADS1015_REG_CONFIG_MODE_CONTIN;   // Single-shot mode (default)

// Set PGA/voltage range
	ADS1015_CONFIG |= ADS1015_REG_CONFIG_PGA_4_096V;
	switch (channel)
	{
		case (0):
		  ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
		  break;
		case (1):
		  ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
		  break;
		case (2):
		  ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
		  break;
		case (3):
		  ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
		  break;
	}
	// Set 'start single-conversion' bit
	//ADS1015_CONFIG |= ADS1015_REG_CONFIG_OS_SINGLE;

	// Write config register to the ADC
	ADS1015_WriteOneByte(i2cAddress, ADS1015_REG_POINTER_CONFIG, ADS1015_CONFIG);

}

void ADS1015_Init()
{
	i2c_init();
	ADS1015_Config(ADS1015_ADDRESS,0);
}

