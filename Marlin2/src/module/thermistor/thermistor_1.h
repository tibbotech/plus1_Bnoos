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

// R25 = 100 kOhm, beta25 = 4092 K, 4.7 kOhm pull-up, bed thermistor
#define OV_12_TO_10(x)		(OV((x)/2))

const temp_entry_t temptable_1[] PROGMEM = {
  { OV_12_TO_10(  23), 320 },
  { OV_12_TO_10(  25), 295 },
  { OV_12_TO_10(  27), 290 },
  { OV_12_TO_10(  28), 285 },
  { OV_12_TO_10(  31), 280 },
  { OV_12_TO_10(  50), 275 },
  { OV_12_TO_10(  53), 270 },
  { OV_12_TO_10(  60), 265 },
  { OV_12_TO_10(  63), 260 },
  { OV_12_TO_10(  70), 255 },
  { OV_12_TO_10(  77), 250 },
  { OV_12_TO_10(  83), 245 },
  { OV_12_TO_10(  88), 240 },
  { OV_12_TO_10(  96), 235 },
  { OV_12_TO_10(  103), 230 },
  { OV_12_TO_10(  113), 225 },
  { OV_12_TO_10(  123), 220 },
  { OV_12_TO_10(  133), 215 },
  { OV_12_TO_10(  146), 210 },
  { OV_12_TO_10( 160), 205 },
  { OV_12_TO_10( 170), 200 },
  { OV_12_TO_10( 193), 195 },
  { OV_12_TO_10( 213), 190 },
  { OV_12_TO_10( 230), 185 },
  { OV_12_TO_10( 246), 180 },
  { OV_12_TO_10( 273), 175 },
  { OV_12_TO_10( 300), 170 },
  { OV_12_TO_10( 326), 165 },
  { OV_12_TO_10( 361), 160 },
  { OV_12_TO_10( 390), 155 },
  { OV_12_TO_10( 426), 150 },
  { OV_12_TO_10( 466), 145 },
  { OV_12_TO_10( 515), 140 },
  { OV_12_TO_10( 561), 135 },
  { OV_12_TO_10( 606), 130 },
  { OV_12_TO_10( 660), 125 },
  { OV_12_TO_10( 716), 120 },
  { OV_12_TO_10( 773), 115 },
  { OV_12_TO_10( 830), 110 },
  { OV_12_TO_10( 890), 105 },
  { OV_12_TO_10( 951), 100 },
  { OV_12_TO_10( 1011),  95 },
  { OV_12_TO_10( 1070),  90 },
  { OV_12_TO_10( 1130),  85 },
  { OV_12_TO_10( 1190),  80 },
  { OV_12_TO_10( 1246),  75 },
  { OV_12_TO_10( 1295),  70 },
  { OV_12_TO_10( 1336),  65 },
  { OV_12_TO_10( 1376),  60 },
  { OV_12_TO_10( 1416),  55 },
  { OV_12_TO_10( 1455),  50 },
  { OV_12_TO_10( 1483),  45 },
  { OV_12_TO_10( 1510),  40 },
  { OV_12_TO_10( 1531),  35 },
  { OV_12_TO_10( 1553),  30 },
  { OV_12_TO_10( 1570),  25 },
  { OV_12_TO_10( 1586),  20 },
  { OV_12_TO_10( 1593),  15 },
  { OV_12_TO_10( 1610),  10 },
  { OV_12_TO_10(1630),   5 },
  { OV_12_TO_10(1650),   0 },
  { OV_12_TO_10(1675),  -5 },
  { OV_12_TO_10(1675), -10 },
  { OV_12_TO_10(1675), -15 }
};
