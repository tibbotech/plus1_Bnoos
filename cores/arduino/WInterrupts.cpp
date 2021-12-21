/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "WInterrupts.h"
#include "interrupt.h"

void attachInterrupt(uint32_t pin, callback_function_t callback, uint32_t mode)
{
	uint32_t it_mode = 0;

	switch (mode)
	{
	case FALLING :
		it_mode = IRQ_MODE_TRIG_EDGE_FALLING;
		break;
	case LOW :
		it_mode = IRQ_MODE_TRIG_LEVEL_LOW;
		break;
	case RISING :
		it_mode = IRQ_MODE_TRIG_EDGE_RISING;
		break;
	case HIGH :
		it_mode = IRQ_MODE_TRIG_LEVEL_HIGH;
		break;
	default:
		it_mode = IRQ_MODE_TRIG_LEVEL_HIGH;
		break;
	}

	sunplus_interrupt_enable(pin, callback, it_mode);
}

void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
	callback_function_t _c = callback;
	attachInterrupt(pin, _c, mode);
}

void detachInterrupt(uint32_t pin)
{
	sunplus_interrupt_disable(pin);
}

