#include "../SPI/src/SPI.h"
#ifdef __cplusplus
extern "C" {
#endif

void spi_test1(void)
{

	SPIClass SPI_1(MOSI,MISO,SCK,SSEL);
	SPI_1.beginTransaction(SPISettings(1500000, MSBFIRST, SPI_MODE3));
	SPI_1.transfer(0x0);
	uint8_t ch = SPI_1.transfer(0x0);
	if(ch != 0xE5)
	{
		printf("[ADXL345] read data error !!!\n");
	}
	SPI_1.transfer16(0x310b);
	SPI_1.transfer16(0x2d08);
	SPI_1.transfer16(0x2c0b);
	SPI_1.transfer16(0x2e00);
	SPI_1.transfer16(0x3800);
	SPI_1.transfer16(0x1e00);
	SPI_1.transfer16(0x1f00);
	SPI_1.transfer16(0x2000);
	int a = 1000000;
	while(a--);

	for(int i=0;i<6;i++)
	{
		ch = SPI_1.transfer(0x32+i);
		ch = SPI_1.transfer(0x0);
		printf("%d %s addr=%x(%x)\n",__LINE__,__FUNCTION__,0x32,ch);
	}
}


#ifdef __cplusplus
}
#endif


