/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "../../inc/MarlinConfigPre.h"
#include "uart.h"
#include "HardwareSerial.h"

#ifdef VIRTIOCON




#else

typedef void (*uart_rx_callback_t)(serial_t * obj);

class MarlinSerial : public HardwareSerial {
public:
  MarlinSerial(void* peripheral, uart_rx_callback_t rx_callback) : HardwareSerial(peripheral), _rx_callback(rx_callback)  { }
  void begin(unsigned long baud);
  void _rx_complete_irq(serial_t* obj);
  bool connected() { return true; }
protected:
  uart_rx_callback_t _rx_callback;
};


extern MarlinSerial MSerial1;
extern MarlinSerial MSerial2;
extern MarlinSerial MSerial3;
extern MarlinSerial MSerial4;
#endif
