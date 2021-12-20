
#include "SPI.h"

SPIClass SPI;

SPIClass::SPIClass() : _CSPinConfig(NO_CONFIG)
{
  _spi.pin_miso = MISO;
  _spi.pin_mosi = MOSI;
  _spi.pin_sclk = SCK;
  _spi.pin_ssel = NC;
}

SPIClass::SPIClass(uint8_t mosi, uint8_t miso, uint8_t sclk, uint8_t ssel) : _CSPinConfig(NO_CONFIG)
{
  _spi.pin_miso = miso;
  _spi.pin_mosi = mosi;
  _spi.pin_sclk = sclk;
  _spi.pin_ssel = ssel;
}


void SPIClass::begin(uint8_t _pin)
{
  uint8_t idx = pinIdx(_pin, ADD_NEW_PIN);
  if (idx >= NB_SPI_SETTINGS) {
    return;
  }

  if ((_pin != CS_PIN_CONTROLLED_BY_USER) && (_spi.pin_ssel == NC)) {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, HIGH);
  }

  _spi.handle.State = HAL_SPI_STATE_RESET;
  spi_init(&_spi, spiSettings[idx].clk,
           spiSettings[idx].dMode,
           spiSettings[idx].bOrder);
  _CSPinConfig = _pin;
#if __has_include("WiFi.h")
  delay(2000);
#endif

}

/**
  * @brief  This function should be used to configure the SPI instance in case you
  *         don't use the default parameters set by the begin() function.
  * @param  _pin: CS pin (optional). This pin will be attached with the settings.
  * @param  settings: SPI settings(clock speed, bit order, data mode).
  * @Note   For each SPI instance you are able to manage until NB_SPI_SETTINGS
  *         devices attached to the same SPI peripheral. You need to indicate the
  *         CS pin used to the transfer() function and the SPI instance will be
  *         configured with the right settings.
  */
void SPIClass::beginTransaction(uint8_t _pin, SPISettings settings)
{
  uint8_t idx = pinIdx(_pin, ADD_NEW_PIN);
  if (idx >= NB_SPI_SETTINGS) {
    return;
  }

  spiSettings[idx].clk = settings.clk;
  spiSettings[idx].dMode = settings.dMode;
  spiSettings[idx].bOrder = settings.bOrder;
  spiSettings[idx].noReceive = settings.noReceive;

  if ((_pin != CS_PIN_CONTROLLED_BY_USER) && (_spi.pin_ssel == NC)) {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, HIGH);
  }

  spi_init(&_spi, spiSettings[idx].clk,
           spiSettings[idx].dMode,
           spiSettings[idx].bOrder);
  _CSPinConfig = _pin;
}

/**
  * @brief  Remove the CS pin and the settings associated to the SPI instance.
  * @param  _pin: CS pin (optional)
  */
void SPIClass::endTransaction(uint8_t _pin)
{
  RemovePin(_pin);
  _CSPinConfig = NO_CONFIG;
}

/**
  * @brief  Deinitialize the SPI instance and stop it.
  */
void SPIClass::end()
{
  spi_deinit(&_spi);
  RemoveAllPin();
  _CSPinConfig = NO_CONFIG;
}

/**
  * @brief  Deprecated function.
  *         Configure the bit order: MSB first or LSB first.
  * @param  _pin: CS pin associated to a configuration (optional).
  * @param  _bitOrder: MSBFIRST or LSBFIRST
  */
void SPIClass::setBitOrder(uint8_t _pin, BitOrder _bitOrder)
{
  uint8_t idx = pinIdx(_pin, GET_IDX);
  if (idx >= NB_SPI_SETTINGS) {
    return;
  }

  spiSettings[idx].bOrder = _bitOrder;

  spi_init(&_spi, spiSettings[idx].clk,
           spiSettings[idx].dMode,
           spiSettings[idx].bOrder);
}

/**
  * @brief  Deprecated function.
  *         Configure the data mode (clock polarity and clock phase)
  * @param  _pin: CS pin associated to a configuration (optional).
  * @param  _mode: SPI_MODE0, SPI_MODE1, SPI_MODE2 or SPI_MODE3
  * @note
  *         Mode          Clock Polarity (CPOL)   Clock Phase (CPHA)
  *         SPI_MODE0             0                     0
  *         SPI_MODE1             0                     1
  *         SPI_MODE2             1                     0
  *         SPI_MODE3             1                     1
  */
void SPIClass::setDataMode(uint8_t _pin, uint8_t _mode)
{
  uint8_t idx = pinIdx(_pin, GET_IDX);
  if (idx >= NB_SPI_SETTINGS) {
    return;
  }

  if (SPI_MODE0 == _mode) {
    spiSettings[idx].dMode = SPI_MODE_0;
  } else if (SPI_MODE1 == _mode) {
    spiSettings[idx].dMode = SPI_MODE_1;
  } else if (SPI_MODE2 == _mode) {
    spiSettings[idx].dMode = SPI_MODE_2;
  } else if (SPI_MODE3 == _mode) {
    spiSettings[idx].dMode = SPI_MODE_3;
  }

  spi_init(&_spi, spiSettings[idx].clk,
           spiSettings[idx].dMode,
           spiSettings[idx].bOrder);
}


void SPIClass::setClockDivider(uint8_t _pin, uint8_t _divider)
{
  
}

byte SPIClass::transfer(uint8_t _pin, uint8_t data, SPITransferMode _mode)
{
  uint8_t rx_buffer = 0;

  uint8_t idx = pinIdx(_pin, GET_IDX);
  if (idx >= NB_SPI_SETTINGS) {
    return rx_buffer;
  }

  if (_pin != _CSPinConfig) {
    spi_init(&_spi, spiSettings[idx].clk,
             spiSettings[idx].dMode,
             spiSettings[idx].bOrder);
    _CSPinConfig = _pin;
  }

  if ((_pin != CS_PIN_CONTROLLED_BY_USER) && (_spi.pin_ssel == NC)) {
    digitalWrite(_pin, LOW);
  }

  spi_transfer(&_spi, &data, &rx_buffer, sizeof(uint8_t), SPI_TRANSFER_TIMEOUT, spiSettings[idx].noReceive);

  if ((_pin != CS_PIN_CONTROLLED_BY_USER) && (_mode == SPI_LAST) && (_spi.pin_ssel == NC)) {
    digitalWrite(_pin, HIGH);
  }

  return rx_buffer;
}

uint16_t SPIClass::transfer16(uint8_t _pin, uint16_t data, SPITransferMode _mode)
{
  uint16_t rx_buffer = 0;
  uint16_t tmp;

  uint8_t idx = pinIdx(_pin, GET_IDX);
  if (idx >= NB_SPI_SETTINGS) {
    return rx_buffer;
  }

  if (_pin != _CSPinConfig) {
    spi_init(&_spi, spiSettings[idx].clk,
             spiSettings[idx].dMode,
             spiSettings[idx].bOrder);
    _CSPinConfig = _pin;
  }

  if (spiSettings[idx].bOrder) {
    tmp = ((data & 0xff00) >> 8) | ((data & 0xff) << 8);
    data = tmp;
  }

  if ((_pin != CS_PIN_CONTROLLED_BY_USER) && (_spi.pin_ssel == NC)) {
    digitalWrite(_pin, LOW);
  }

  spi_transfer(&_spi, (uint8_t *)&data, (uint8_t *)&rx_buffer, sizeof(uint16_t),
               SPI_TRANSFER_TIMEOUT, spiSettings[idx].noReceive);

  if ((_pin != CS_PIN_CONTROLLED_BY_USER) && (_mode == SPI_LAST) && (_spi.pin_ssel == NC)) {
    digitalWrite(_pin, HIGH);
  }

  if (spiSettings[idx].bOrder) {
    tmp = ((rx_buffer & 0xff00) >> 8) | ((rx_buffer & 0xff) << 8);
    rx_buffer = tmp;
  }

  return rx_buffer;
}


void SPIClass::transfer(uint8_t _pin, void *_buf, size_t _count, SPITransferMode _mode)
{
  if ((_count == 0) || (_buf == NULL)) {
    return;
  }
  uint8_t idx = pinIdx(_pin, GET_IDX);
  if (idx >= NB_SPI_SETTINGS) {
    return;
  }
  if (_pin != _CSPinConfig) {

    spi_init(&_spi, spiSettings[idx].clk,
             spiSettings[idx].dMode,
             spiSettings[idx].bOrder);
    _CSPinConfig = _pin;
  }

  if ((_pin != CS_PIN_CONTROLLED_BY_USER) && (_spi.pin_ssel == NC)) {
    digitalWrite(_pin, LOW);
  }

  spi_transfer(&_spi, ((uint8_t *)_buf), ((uint8_t *)_buf), _count,
               SPI_TRANSFER_TIMEOUT, spiSettings[idx].noReceive);

  if ((_pin != CS_PIN_CONTROLLED_BY_USER) && (_mode == SPI_LAST) && (_spi.pin_ssel == NC)) {
    digitalWrite(_pin, HIGH);
  }
}


void SPIClass::transfer(byte _pin, void *_bufout, void *_bufin, size_t _count, SPITransferMode _mode)
{
  if ((_count == 0) || (_bufout == NULL) || (_bufin == NULL)) {
    return;
  }
  uint8_t idx = pinIdx(_pin, GET_IDX);
  if (idx >= NB_SPI_SETTINGS) {
    return;
  }

  if (_pin != _CSPinConfig) {
    spi_init(&_spi, spiSettings[idx].clk,
             spiSettings[idx].dMode,
             spiSettings[idx].bOrder);
    _CSPinConfig = _pin;
  }

  if ((_pin != CS_PIN_CONTROLLED_BY_USER) && (_spi.pin_ssel == NC)) {
    digitalWrite(_pin, LOW);
  }

  spi_transfer(&_spi, ((uint8_t *)_bufout), ((uint8_t *)_bufin), _count,
               SPI_TRANSFER_TIMEOUT, spiSettings[idx].noReceive);

  if ((_pin != CS_PIN_CONTROLLED_BY_USER) && (_mode == SPI_LAST) && (_spi.pin_ssel == NC)) {
    digitalWrite(_pin, HIGH);
  }
}

void SPIClass::usingInterrupt(uint8_t interruptNumber)
{
  UNUSED(interruptNumber);
}

void SPIClass::attachInterrupt(void)
{
  
}

void SPIClass::detachInterrupt(void)
{

}
