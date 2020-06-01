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
 * Creation date: 28 Jul 2020
 */

#ifndef _QUANTUM_BINARY_INPUT_H
#define _QUANTUM_BINARY_INPUT_H

#include <Arduino.h>

/**
 * Quantum event codes
 */
enum QUANTUM_PIN_TYPE
{
  QUANTUM_PIN_TYPE_BINARY = 0,
  QUANTUM_PIN_TYPE_COUNTER
};

/**
 * Main class
 */
class QUANTUM_BINARY_INPUT
{
  private:

    /**
     * Minimum timeout (msec) between activations
     */
    uint32_t minTimeout;

  public:

    /**
     * Pin number
     */
    uint8_t pinNumber;

    /**
     * Pin type (QUANTUM_PIN_TYPE)
     */
    uint8_t type;

    /**
     * Time stamp of last update in msec
     */
    uint32_t timeStamp;

    /**
     * Pin state. 0: off, 1: on
     */
    uint8_t state;

    /**
     * Elapsed time since last change of state
     */
    uint32_t elapsedTime;

    /**
     * Class constructor
     *
     * @param pin Pin number
     * @param timeout Minimum timeout between activations
     */
    inline QUANTUM_BINARY_INPUT(const uint8_t pin, const uint32_t timeout=0)
    {
      pinNumber = pin;
      type = QUANTUM_PIN_TYPE_BINARY;
      minTimeout = timeout;
      state = 0;
      timeStamp = 0;
      elapsedTime = 0;
    }

    /**
     * begin
     *
     * Initialize binary pin
     */
    inline void begin(void)
    {
      pinMode(pinNumber, INPUT);
      state = !digitalRead(pinNumber);
    }

    /**
     * update
     *
     * Update pin state
     *
     * @return Return true in case of change of state
     */
    inline bool update(void)
    {
      bool newState = !digitalRead(pinNumber);
      uint32_t now = millis();
      
      if (newState != state)
      {
        elapsedTime = now - timeStamp;
        if (elapsedTime > minTimeout)
        {
          state = newState;
          timeStamp = now;
          return true;
        }
      }
      else
        timeStamp = now;

      return false;
    }
};

#endif

