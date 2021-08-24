/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
 * Copyright (c) 2017 Victor Perez
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

#define CPU_32_BIT

#include "../../core/macros.h"
#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "fastio.h"
#include "watchdog.h"
#include "MarlinSerial.h"

#include "../../inc/MarlinConfigPre.h"
#include "temperature_ads1015.h"
#include <stdint.h>


#include "uart.h"


#ifdef USBCON
  #include <USBSerial.h>
  #include "../../core/serial_hook.h"
  typedef ForwardSerial1Class< decltype(SerialUSB) > DefaultSerial1;
  extern DefaultSerial1 MSerial0;
#endif

// ------------------------
// Defines
// ------------------------
#define _MSERIAL(X) MSerial##X
#define MSERIAL(X) _MSERIAL(X)

#if SERIAL_PORT == -1
  #define MYSERIAL0 SerialUSB
#elif WITHIN(SERIAL_PORT, 1, 4)
  #define MYSERIAL0 Serial
#else
  #error "SERIAL_PORT must be -1 or from 1 to 4. Please update your configuration."
#endif

#ifdef SERIAL_PORT_2
  #if SERIAL_PORT_2 == -1
    #define MYSERIAL1 SerialUSB
  #elif WITHIN(SERIAL_PORT_2, 1, 4)
    #define MYSERIAL1 MSERIAL(SERIAL_PORT_2)
  #else
    #error "SERIAL_PORT_2 must be -1 or from 1 to 4. Please update your configuration."
  #endif
#endif

#ifdef LCD_SERIAL_PORT
  #if LCD_SERIAL_PORT == -1
    #define LCD_SERIAL SerialUSB
  #elif WITHIN(LCD_SERIAL_PORT, 1, 4)
    #define LCD_SERIAL MSERIAL(LCD_SERIAL_PORT)
  #else
    #error "LCD_SERIAL_PORT must be -1 or from 1 to 4. Please update your configuration."
  #endif
  #if HAS_DGUS_LCD
    #define SERIAL_GET_TX_BUFFER_FREE() LCD_SERIAL.availableForWrite()
  #endif
#endif

/**
 * TODO: review this to return 1 for pins that are not analog input
 */
#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) (p)
#endif

#define CRITICAL_SECTION_START()  uint32_t primask = __get_PRIMASK(); __disable_irq()
#define CRITICAL_SECTION_END()    if (!primask) __enable_irq()
#define ISRS_ENABLED() (!__get_PRIMASK())
#define ENABLE_ISRS()  //__enable_irq()
#define DISABLE_ISRS() //__disable_irq()
#define cli() __disable_irq()
#define sei() __enable_irq()

// On AVR this is in math.h?
#define square(x) ((x)*(x))

#ifndef strncpy_P
  #define strncpy_P(dest, src, num) strncpy((dest), (src), (num))
#endif

// Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*(addr))


#define HAL_ADC_RESOLUTION  10

// ------------------------
// Types
// ------------------------

typedef int16_t pin_t;

// ------------------------
// Public Variables
// ------------------------

// result of last ADC conversion
extern volatile uint16_t bed_adc_result;
extern volatile uint16_t hotend_adc_result;
extern volatile uint16_t last_adc_pin;

// ------------------------
// Public functions
// ------------------------

// Memory related
#define __bss_end __bss_end__

// Enable hooks into  setup for HAL
void HAL_init();

// Clear reset reason
void HAL_clear_reset_source();

// Reset reason
uint8_t HAL_get_reset_source();

inline void HAL_reboot() {}  // reboot the board or restart the bootloader

void _delay_ms(const int delay);

extern "C" char* _sbrk(int incr);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static inline int freeMemory() {
  //volatile char top;
  return 0;//&top - reinterpret_cast<char*>(_sbrk(0));
}

#pragma GCC diagnostic pop

#define HAL_ANALOG_SELECT(pin) //pinMode(pin, INPUT)

#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC()      (last_adc_pin == TEMP_0_PIN? hotend_adc_result:bed_adc_result)
#define HAL_ADC_READY()     true

// ------------------------
// ADC
// ------------------------
inline void HAL_adc_init() 
{
    // init i2c device 
}

void HAL_adc_start_conversion(const uint8_t adc_pin);

uint16_t HAL_adc_get_result();

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)


#define PLATFORM_M997_SUPPORT
void flashFirmware(const int16_t);

// Maple Compatibility
typedef void (*systickCallback_t)(void);
void systick_attach_callback(systickCallback_t cb);
void HAL_SYSTICK_Callback();
extern volatile uint32_t systick_uptime_millis;

