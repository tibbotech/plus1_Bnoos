
#ifdef ARDUINO_ARCH_SP7021

#include "../../inc/MarlinConfig.h"
//#include <Wire.h>
#include "temperature_ads1015.h"
#include <Wire.h>
#include "module/temperature.h"

static int temp_result[4] = {0};
//#define I2C_GPIO

#ifdef I2C_GPIO

#define I2C_NUM		0  /* contain 4 I2C,from 0--3 */

unsigned short ADS1015_CONFIG;//定义ADS1015配置变量

#define TEMP_I2C_SDA		12
#define TEMP_I2C_SCL		13



#define GPIO_PIN_INIT(gpio_index)		OUT_WRITE(gpio_index,1)

#define I2C_SDA_1()						WRITE(TEMP_I2C_SDA,1)
#define I2C_SDA_0()						WRITE(TEMP_I2C_SDA,0)

#define I2C_SCL_1()						WRITE(TEMP_I2C_SCL,1)
#define I2C_SCL_0()						WRITE(TEMP_I2C_SCL,0)

#define I2C_SDA_READ()					READ(TEMP_I2C_SDA)

#define I2C_SDA_IN()					gpio_oe_val_set(TEMP_I2C_SDA,0)
#define I2C_SDA_OUT()					gpio_oe_val_set(TEMP_I2C_SDA,1)

//#define Delay()							STC_delay_us(15)


void Delay()
{
	int s = micros();
	while(micros()-s<15);
}
void i2c_init(void)
{
	GPIO_PIN_INIT(TEMP_I2C_SDA);
	GPIO_PIN_INIT(TEMP_I2C_SCL);
	printf("\n SCL=%d  SDA=%d \n",TEMP_I2C_SCL,TEMP_I2C_SDA);
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

	if(cnt == 0)
		printf("i2c slave no ack !!!\n");
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

	int ret = i2c_read_ack();
	if(ret)
		printf("write data error !!\n");
	return ret;

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

extern float current_temperature[1] ;
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
    ADS1015_CONFIG |= ADS1015_REG_CONFIG_PGA_6_144V;
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
		temp=temp/2;

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
	ADS1015_CONFIG |= ADS1015_REG_CONFIG_PGA_6_144V;
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

volatile int temp1_hot=1586,temp1_bed=1586;
int xy = 0;
void i2c_get()
{

	temp1_hot = ADS1015_Read(2);
	temp1_bed = ADS1015_Read(1);
}

#else


#define I2C_NUM		0  /* contain 4 I2C,from 0--3 */

unsigned short ADS1015_CONFIG2;

void i2c_delay_us(int time)
{
	int s = micros();
	while(micros() - s < time);
}

//ADS1015 read 2bytes data from reg
unsigned short ADS1015_ReadOneByte(unsigned char i2cAddress,unsigned short ReadAddr)
{

	Wire.beginTransmission(i2cAddress);
	Wire.write(ADS1015_REG_POINTER_CONVERT);
	Wire.endTransmission();
	Wire.requestFrom(i2cAddress, 2);
	return ((Wire.read() << 8) | Wire.read());

}

//ADS1015 write data to reg
void ADS1015_WriteOneByte(unsigned char i2cAddress,unsigned short WriteAddr,unsigned short DataToWrite)
{


	Wire.beginTransmission(i2cAddress); 
  	Wire.write(WriteAddr);        
  	Wire.write(DataToWrite>>8);
  	Wire.write(DataToWrite&0xff);                
  	Wire.endTransmission();    

}

extern float current_temperature[1] ;
unsigned short ADS1015_Read(unsigned char channel)
{
int temp;

 // Start with default values
  ADS1015_CONFIG2  = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
					ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
					ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
					ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
					ADS1015_REG_CONFIG_DR_3300SPS   | // 1600 samples per second (default)
					ADS1015_REG_CONFIG_MODE_CONTIN;   // continue mode (default)
	
    // Set PGA/voltage range
    ADS1015_CONFIG2 |= ADS1015_REG_CONFIG_PGA_6_144V;
	if(channel<4)
	{
		switch (channel)
		{
			case (0):
				ADS1015_CONFIG2 |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
			break;
			case (1):
				ADS1015_CONFIG2 |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
			break;
			case (2):
				ADS1015_CONFIG2 |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
			break;
			case (3):
				ADS1015_CONFIG2 |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
			break;
		}
		  // Set 'start single-conversion' bit
		ADS1015_WriteOneByte(ADS1015_ADDRESS,ADS1015_REG_POINTER_CONFIG, ADS1015_CONFIG2);
		i2c_delay_us(200);
		temp=ADS1015_ReadOneByte(ADS1015_ADDRESS,ADS1015_REG_POINTER_CONVERT);
		temp=temp>>4;
		temp = temp/2; // 12bit --> 10bit
		return temp;
	}
return 0;
}

void ADS1015_Config(unsigned char i2cAddress,unsigned char channel)
{
// Start with default values
ADS1015_CONFIG2  = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
				ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
				ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
				ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
				ADS1015_REG_CONFIG_DR_3300SPS   | // 1600 samples per second (default)
				ADS1015_REG_CONFIG_MODE_CONTIN;   // Single-shot mode (default)

// Set PGA/voltage range
	ADS1015_CONFIG2 |= ADS1015_REG_CONFIG_PGA_6_144V;
	switch (channel)
	{
		case (0):
		  ADS1015_CONFIG2 |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
		  break;
		case (1):
		  ADS1015_CONFIG2 |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
		  break;
		case (2):
		  ADS1015_CONFIG2 |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
		  break;
		case (3):
		  ADS1015_CONFIG2 |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
		  break;
	}
	// Set 'start single-conversion' bit
	//ADS1015_CONFIG2 |= ADS1015_REG_CONFIG_OS_SINGLE;

	// Write config register to the ADC
	ADS1015_WriteOneByte(i2cAddress, ADS1015_REG_POINTER_CONFIG, ADS1015_CONFIG2);

}

void ADS1015_Init()
{
	Wire.begin();

	ADS1015_Config(ADS1015_ADDRESS,0);
}
int xtdebug = 0;
void ADS1015_Handler()
{
#if 0
	xtdebug++;
	if (xtdebug % 200 == 0)
	{
		printf_a("live\n");
	}
#endif
	//temp_result[0] = ADS1015_Read(0);
	temp_result[1] = ADS1015_Read(1);
	temp_result[2] = ADS1015_Read(2);
	//temp_result[3] = ADS1015_Read(3);
}

unsigned short ADS1015_Get_Data(unsigned char channel)
{
	int temp;
	if(channel<4)
	{
		switch (channel)
		{
			case (0):
				temp = temp_result[0];
			break;
			case (1):
				temp = temp_result[1];
			break;
			case (2):
				temp = temp_result[2];
			break;
			case (3):
				temp = temp_result[3];
			break;
		}
		return temp;
	}
	return 0;
}

#endif


#endif
