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

#pragma once


#define BOARD_INFO_NAME "SP7021"

#define ALFAWISE_UX0					// heater gpio use OD mode

//
// Timers
//
#define STEP_TIMER                             2
#define TEMP_TIMER                             3
#define ADS1015_TIMER						   4

//
// Limit Switches
//
#define X_MIN_PIN                           GPIO_P2_04
#define Y_MIN_PIN                           GPIO_P2_05
#define Z_MIN_PIN                           GPIO_P3_00

//
// Steppers
//
#define X_STEP_PIN                          GPIO_P4_07
#define X_DIR_PIN                           GPIO_P4_05
#define X_ENABLE_PIN                        GPIO_P4_06

#define Y_STEP_PIN                          GPIO_P4_03
#define Y_DIR_PIN                           GPIO_P4_04
#define Y_ENABLE_PIN                        GPIO_P3_07

#define Z_STEP_PIN                          GPIO_P4_00
#define Z_DIR_PIN                           GPIO_P4_02
#define Z_ENABLE_PIN                        GPIO_P4_01

#define E0_STEP_PIN                         GPIO_P3_02
#define E0_DIR_PIN                          GPIO_P3_04
#define E0_ENABLE_PIN                       GPIO_P3_05

//
// Temperature Sensors
//
#define TEMP_0_PIN                          0xFF   // 0xff: for ads1015 channel select
#define TEMP_BED_PIN                        0xFE   // 0xfe: for ads1015 channel select

//
// Heaters / Fans
//
#define HEATER_0_PIN                        GPIO_P2_01   // HOTEND0 MOSFET
#define HEATER_BED_PIN                      GPIO_P2_02   // BED MOSFET

#define AUTO_FAN_PIN                        0xFF
