/*
 * Copyright (c) 2020 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 28 May 2020
 */

#ifndef _QUANTUM_CURRENT_INPUT_H
#define _QUANTUM_CURRENT_INPUT_H

#include <Arduino.h>

/**
 * Main class
 */
class QUANTUM_CURRENT_INPUT
{
  public:

    /**
     * Pin number
     */
    uint8_t pinNumber;

    /**
     * Class constructor
     *
     * @param pin Pin number
     */
    inline QUANTUM_CURRENT_INPUT(const uint8_t pin)
    {
      pinNumber = pin;
    }

    /**
     * read
     *
     * Read 4-20mA current loop
     *
     * @return milliamps read
     */
    inline float read(void)
    {
      float vref = STM32.getVREF();
      float adc = analogRead(pinNumber);

      return adc * vref / (0xFFF * 160);
    }
};

#endif

