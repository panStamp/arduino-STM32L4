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

#ifndef _QUANTUM_BINARY_OUTPUT_H
#define _QUANTUM_BINARY_OUTPUT_H

#include <Arduino.h>

/**
 * Main class
 */
class QUANTUM_BINARY_OUTPUT
{
  public:

    /**
     * Pin number
     */
    uint8_t pinNumber;

    /**
     * Pin state. 0: off, 1: on
     */
    uint8_t state;    

    /**
     * Class constructor
     *
     * @param pin Pin number
     */
    inline QUANTUM_BINARY_OUTPUT(const uint8_t pin)
    {
      pinNumber = pin;
    }

    /**
     * begin
     *
     * Initialize binary pin
     */
    inline void begin(void)
    {
      pinMode(pinNumber, OUTPUT);
      state = !digitalRead(pinNumber);
    }

    /**
     * write
     *
     * Set output pin state
     *
     * @param newState : new output state
     */
    inline void write(uint8_t newState)
    {
      digitalWrite(pinNumber, newState);
    }

    /**
     * toggle
     *
     * Toggle output state
     */
    inline void toggle(void)
    {
      digitalWrite(pinNumber, !digitalRead(pinNumber));
    }
};

#endif

