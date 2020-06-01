/*
 * Copyright (c) 2019 panStamp <contact@panstamp.com>
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
 * Creation date: 12 Jul 2019
 */

#ifndef _QUANTUM_BACKUP_H_
#define _QUANTUM_BACKUP_H_

#include <stm32l4_rtc.h>

/**
 * SRAM backup space
 */
enum QUANTUM_BACKUP_SPACE
{
  QUANTUM_BACKUP_HEADER = 0,
  QUANTUM_BACKUP_COUNTER1
};

const uint32_t QUANTUM_BACKUP_SPACE_INITIALIZED = 0xA1B2C3D4;

/**
 * Main class
 */
class QUANTUM_BACKUP
{
  public:

    /**
     * begin
     *
     * Initialize backup space
     */
    inline void begin(void)
    {
      // Backup space not yet initialized?
      if (stm32l4_rtc_read_backup(QUANTUM_BACKUP_HEADER) != QUANTUM_BACKUP_SPACE_INITIALIZED)
      {
        for(uint8_t i=0 ; i<32 ; i++)
          stm32l4_rtc_write_backup(i, 0);

        // Set backup space as initialized
        stm32l4_rtc_write_backup(QUANTUM_BACKUP_HEADER, QUANTUM_BACKUP_SPACE_INITIALIZED);
      }
    }

    /**
     * save
     *
     * Save uint32_t value in backup space
     *
     * @param addr Address in backup space
     * @param val uint32_t value
     */
    inline void save(uint8_t addr, uint32_t val)
    {
      stm32l4_rtc_write_backup(addr, val);
    }

    /**
     * read
     *
     * Read uint32_t value from backup space
     *
     * @param addr Address in backup space
     *
     * @return uint32_t value
     */
    inline uint32_t read(uint8_t addr)
    {
      return stm32l4_rtc_read_backup(addr);
    }
};

#endif

