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

#ifndef _QUANTUM_COUNTER_INPUT_
#define _QUANTUM_COUNTER_INPUT_

#include "quantum_binary_input.h"
#include "quantum_backup.h"

/**
 * Main class
 */
class QUANTUM_COUNTER_INPUT : public QUANTUM_BINARY_INPUT
{
  private:

    /**
     * Address in backup space
     */
    uint8_t backupAddress;

    /**
     * Static index of counter
     */
    static uint8_t index;

    /**
     * Backup space
     */
    QUANTUM_BACKUP backupSpace;

  public:

    /**
     * Active level (LOW or HIGH)
     */
    bool activeLevel;

    /**
     * Backup flag. Save in backup space if this flag is enabled
     */
    bool backup;

    /**
     * Counter value
     */
    uint32_t counter;

    /**
     * Class constructor
     *
     * @param pin Pin number
     * @param level Active level
     * @param timeout Minimum timeout between activations
     * @param save Save in backup space
     */
    inline QUANTUM_COUNTER_INPUT(const uint8_t pin, const bool level, const uint32_t timeout=0, const bool save=false) : QUANTUM_BINARY_INPUT(pin, timeout)
    {
      backupAddress = ++index;
      type = QUANTUM_PIN_TYPE_COUNTER;
      activeLevel = level;
      backup = save;
      counter = 0;
    }

    /**
     * begin
     *
     * Initialize counter. Read initial value from backup space
     */
    inline void begin(void)
    {
      QUANTUM_BINARY_INPUT::begin();

      if (backup)
        counter = backupSpace.read(backupAddress);
    }

    /**
     * update
     *
     * Update pin state and counter
     *
     * @return Return true in case of change of state
     */
    inline bool update(void)
    {
      bool res = QUANTUM_BINARY_INPUT::update();

      if (res)
      {
        if (state == activeLevel)
        {
          counter++;
          
          if (backup)
            backupSpace.save(backupAddress, counter);

          return true;
        }
      }

      return false;
    }
};

#endif

