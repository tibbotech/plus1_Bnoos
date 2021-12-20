#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

#include <stdbool.h>
#include <stdlib.h>
#include "variant.h"

static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;
static const uint8_t SSEL = PIN_SPI_CS;

static const uint8_t SDA = PIN_I2C_SDA;
static const uint8_t SCL = PIN_I2C_SCL;

#define NC    -1

#endif
