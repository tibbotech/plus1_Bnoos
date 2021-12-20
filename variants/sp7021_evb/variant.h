//void SystemClock_Config(void);
#define SERIAL_UART_INSTANCE	2

/* select spi.(SPI0-SPI4) */
//#define SPI_SEL_INSTANCE	1

/* select i2c.(I2C0-I2C3) */
//#define I2C_SEL_INSTANCE	0

/* select icm.(ICM0--ICM3) */
//#define ICM_SEL_INSTANCE    0

/* select ext_int.(EXT_INT0--EXT_INT7) */
//#define EXT_INT_SEL_INSTANCE	0

#define PIN_SERIAL_TX	GPIO_P1_01
#define PIN_SERIAL_RX	GPIO_P1_00

#define PIN_SPI_MISO	GPIO_P8_07
#define PIN_SPI_MOSI	GPIO_P8_07
#define PIN_SPI_SCK 	GPIO_P8_07
#define PIN_SPI_CS 		GPIO_P8_07

#define PIN_I2C_SDA		GPIO_P1_04
#define PIN_I2C_SCL		GPIO_P1_05

#define PIN_ICM_DATA
#define PIN_ICM_CLK

#define PIN_EXT_INT0	GPIO_P4_07

#define NUM_DIGITAL_PINS	(GPIO_P8_07+1)

#define PIN_ICM_DATA	GPIO_P2_03
#define PIN_ICM_CLK		-1

