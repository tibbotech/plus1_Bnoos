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
#ifdef ARDUINO_ARCH_SP7021

#include "HAL.h"
//#include "usb_serial.h"

#include "../../inc/MarlinConfig.h"
#include "../shared/Delay.h"

#define THERMISTOR_TABLE_ADC_RESOLUTION 10
#define THERMISTOR_TABLE_SCALE (HAL_ADC_RANGE / _BV(THERMISTOR_TABLE_ADC_RESOLUTION))
#define OVERSAMPLENR 16
#define OV_SCALE(N) (N)
#define OV(N) int16_t(OV_SCALE(N) * (OVERSAMPLENR) * (THERMISTOR_TABLE_SCALE))
#define OV_12_TO_10(x)		(OV((x)/2))


// ------------------------
// Public Variables
// ------------------------
volatile uint16_t bed_adc_result=0; 
volatile uint16_t hotend_adc_result=0;
volatile uint16_t last_adc_pin=0;
// ------------------------
// Public functions
// ------------------------

// Needed for DELAY_NS() / DELAY_US() on CORTEX-M7
#if (defined(__arm__) || defined(__thumb__)) && __CORTEX_M == 7
  // HAL pre-initialization task
  // Force the preinit function to run between the premain() and main() function
  // of the STM32 arduino core
  __attribute__((constructor (102)))
  void HAL_preinit() {
    enableCycleCounter();
  }
#endif

// HAL initialization task
void HAL_init() {

  #if ENABLED(SDSUPPORT)
    OUT_WRITE(SDSS, HIGH);  // Try to set SDSS inactive before any other SPI users start up
  #endif

  #if PIN_EXISTS(LED)
    OUT_WRITE(LED_PIN, LOW);
  #endif

    ADS1015_Init();
	//ADS1015_TIMER_FREQUENCY = 10Hz. Get Data every 100ms from temperature sensor.
	HAL_timer_start(ADS1015_TIMER_NUM, ADS1015_TIMER_FREQUENCY);
}

void HAL_clear_reset_source() 
{ 

}

uint8_t HAL_get_reset_source() 
{
  //RST_WATCHDOG/RST_SOFTWARE/RST_EXTERNAL/RST_POWER_ON
  return RST_POWER_ON;
}

void _delay_ms(const int delay_ms) { delay(delay_ms); }

extern "C" {
  extern unsigned int _ebss; // end of bss section
}


// TODO: Make sure this doesn't cause any delay
void HAL_adc_start_conversion(const uint8_t adc_pin)
{  
  /*
   sp7021 use ads1015 for temperature sensor. 
   channel 2 for temp extruder.
   channel 1 for temp bed
  */
  if(adc_pin < 0xFE)
  {
  	SERIAL_ECHOLN("adc pin set error");
	return;
  }

  last_adc_pin = adc_pin;
  
  if(adc_pin == TEMP_0_PIN)
  	{
  	//hotend_adc_result = ADS1015_Read(2);
	hotend_adc_result = ADS1015_Get_Data(2);// read data from I2C device
	//printf("hotend_adc_result :%d\n", hotend_adc_result);
	//hotend_adc_result = OV_12_TO_10(170);
  	}
  else  if(adc_pin == TEMP_BED_PIN)
  	{
  	//bed_adc_result = ADS1015_Read(1);
	bed_adc_result = ADS1015_Get_Data(1);// read data from I2C device
	//printf("bed_adc_result :%d\n", bed_adc_result);
	//bed_adc_result = OV_12_TO_10(1376);
  	}
}
uint16_t HAL_adc_get_result() 
{ 
  return (last_adc_pin == TEMP_0_PIN? hotend_adc_result:bed_adc_result);
}

// Reset the system (to initiate a firmware flash)
void flashFirmware(const int16_t) 
{ 
  
}

// Maple Compatibility
volatile uint32_t systick_uptime_millis = 0;
systickCallback_t systick_user_callback;
void systick_attach_callback(systickCallback_t cb) 
{ 
  systick_user_callback = cb; 
}

void HAL_SYSTICK_Callback() 
{
  systick_uptime_millis++;
  if (systick_user_callback)
  {
    systick_user_callback();
  } 
    
}

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
