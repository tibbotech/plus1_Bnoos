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

#include <Arduino.h>

#ifndef NUM_DIGITAL_PINS
   #error "Expected NUM_DIGITAL_PINS not found"
#endif


#define _PIN_ADD_2(NAME_ALPHA, ARDUINO_NUM) { {NAME_ALPHA}, ARDUINO_NUM },
#define _PIN_ADD(NAME_ALPHA, ARDUINO_NUM) { NAME_ALPHA, ARDUINO_NUM },
#define PIN_ADD(NAME) _PIN_ADD(#NAME, NAME)

typedef struct {
  char Port_pin_alpha[5];
  pin_t Ard_num;
} XrefInfo;

////////////////////////////////////////////////////////////

#define MODE_PIN_INPUT  0 // Input mode (reset state)
#define MODE_PIN_OUTPUT 1 // General purpose output mode

#define PIN_NUM(P) (P & 0x000F)
#define PIN_NUM_ALPHA_LEFT(P) (((P & 0x000F) < 10) ? ('0' + (P & 0x000F)) : '1')
#define PIN_NUM_ALPHA_RIGHT(P) (((P & 0x000F) > 9)  ? ('0' + (P & 0x000F) - 10) : 0 )
#define PORT_NUM(P) ((P  >> 4) & 0x0007)
#define PORT_ALPHA(P) ('A' + (P  >> 4))

/**
 * Translation of routines & variables used by pinsDebug.h
 */
#define NUMBER_PINS_TOTAL NUM_DIGITAL_PINS
#define VALID_PIN(ANUM) ((ANUM) >= 0 && (ANUM) < NUMBER_PINS_TOTAL)
#define digitalRead_mod(Ard_num) extDigitalRead(Ard_num)  // must use Arduino pin numbers when doing reads
#define PRINT_PIN(Q)
#define PRINT_PORT(ANUM) port_print(ANUM)
#define DIGITAL_PIN_TO_ANALOG_PIN(ANUM) -1  // will report analog pin number in the print port routine

// x is a variable used to search pin_array
#define GET_ARRAY_IS_DIGITAL(x) ((bool) pin_array[x].is_digital)
#define GET_ARRAY_PIN(x) ((pin_t) pin_array[x].pin)
#define PRINT_ARRAY_NAME(x) do{ sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer); }while(0)
#define MULTI_NAME_PAD 33 // space needed to be pretty if not first name assigned to a pin

#define get_pin_mode(ANUM)    getpinMode(Ard_num)

bool GET_PINMODE(const pin_t Ard_num) 
{
   uint32_t mode =  getpinMode(Ard_num);
   return (mode == GPIO_INPUT_MODE ? MODE_PIN_INPUT : MODE_PIN_OUTPUT);
}

int8_t digital_pin_to_analog_pin(pin_t Ard_num) 
{
  return -1;
}

bool IS_ANALOG(const pin_t Ard_num) { return false; }

bool is_digital(const pin_t x) {
  const uint8_t pin_mode = get_pin_mode(pin_array[x].pin);
  return pin_mode == MODE_PIN_INPUT || pin_mode == MODE_PIN_OUTPUT;
}

void port_print(const int8_t pin) {
  char buffer[16];
  sprintf_P(buffer, "pin name:", pin);
  SERIAL_ECHO(buffer);
}

bool pwm_status(const pin_t Ard_num) { return false; }

void pwm_details(const pin_t Ard_num) { } // pwm_details
