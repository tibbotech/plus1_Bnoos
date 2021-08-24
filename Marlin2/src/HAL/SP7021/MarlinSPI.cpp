/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#if defined(ARDUINO_ARCH_SP7021)

#include "MarlinSPI.h"

static void spi_init(spi_t *obj, uint32_t speed, spi_mode_e mode, uint8_t msb, uint32_t dataSize) {
  spi_init(obj, speed, mode, msb);
}

void MarlinSPI::setClockDivider(uint8_t _div) {
  _speed = spi_getClkFreq(&_spi);// / _div;
  _clockDivider = _div;
}

void MarlinSPI::begin(void) {
  //TODO: only call spi_init if any parameter changed!!
  spi_init(&_spi, _speed, _dataMode, _bitOrder);
}


byte MarlinSPI::transfer(uint8_t _data) {

  return _data;
}

uint8_t MarlinSPI::dmaTransfer(const void *transmitBuf, void *receiveBuf, uint16_t length) {
 

  return 1;
}

uint8_t MarlinSPI::dmaSend(const void * transmitBuf, uint16_t length, bool minc) {

  return 1;
}

#endif // ARDUINO_ARCH_SP7021
