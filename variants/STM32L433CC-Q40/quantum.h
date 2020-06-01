/**
 * Copyright (c) 2020 panStamp <contact@panstamp.com>
 * 
 * This file is part of the quantum project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 1 Jun 2020
 */

#ifndef _QUANTUM_H
#define _QUANTUM_H

#include <SPI.h>

class QUANTUM
{
  public:
    /**
     * 8-byte Device EUI
     */
    uint8_t devEui[8];

    /**
     * begin
     *
     * Initialize quantum object
     */
    inline void begin(void)
    {
      // BOOT0 button
      pinMode(BOOT0, INPUT);

      // On-board LED
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, LOW);

      // Take DEVEUI from internal UID
      uint32_t uid[3];
      STM32.getUID(uid);

      // Little Endian
      devEui[7] = (uint8_t)((uid[1] >> 24) & 0xFF);
      devEui[6] = (uint8_t)((uid[1] >> 16) & 0xFF);
      devEui[5] = (uint8_t)((uid[1] >> 8) & 0xFF);
      devEui[4] = (uint8_t)(uid[1] & 0xFF);
      devEui[3] = (uint8_t)((uid[0] >> 24) & 0xFF);
      devEui[2] = (uint8_t)((uid[0] >> 16) & 0xFF);
      devEui[1] = (uint8_t)((uid[0] >> 8) & 0xFF);
      devEui[0] = (uint8_t)(uid[0] & 0xFF);
    }

    /**
     * stopSec
     * 
     * Sleep (stop mode, 1.28 uA) for a given period of time
     * 
     * @param sec seconds to sleep
     */
    inline static void stopSec(uint16_t sec)
    {
      uint32_t msec = sec * 1000;

      SPI.end();
      digitalWrite(SCK, LOW); // SCK
      pinMode(SCK, OUTPUT);
      digitalWrite(MOSI, LOW); // MOSI
      pinMode(MOSI, OUTPUT);

      //pinMode(RF_DIO0, INPUT_ANALOG);
      //pinMode(RF_DIO1, INPUT_ANALOG);
        
      STM32.stop(msec);
      
      SPI.begin();
    }

    /**
     * getVcc
     *
     * @return Voltage supply in millivolts
     */
    inline uint16_t getVcc(void)
    {
      return (uint16_t) (STM32.getVBAT() * 1000);     
    }

    /**
     * standbySec
     * 
     * Stay in standby mode (280 nA) for a given period of time and then restart
     * 
     * @param sec seconds to stay in low power mode
     */
    inline static void standbySec(uint16_t sec)
    {
      uint32_t msec = sec * 1000;

      SPI.end();
      digitalWrite(SCK, LOW); // SCK
      pinMode(SCK, OUTPUT);
      digitalWrite(MOSI, LOW); // MOSI
      pinMode(MOSI, OUTPUT);

      STM32.standby(msec);
      
      SPI.begin();
    }

    /**
     * shutdownSec
     * 
     * Stay in shutdown mode (20 nA) for a given period of time and then restart
     * BOR is disabled in shutdown mode
     * 
     * @param sec seconds to stay in low power mode
     */
    inline static void shutdownSec(uint16_t sec)
    {
      uint32_t msec = sec * 1000;

      SPI.end();
      digitalWrite(SCK, LOW); // SCK
      pinMode(SCK, OUTPUT);
      digitalWrite(MOSI, LOW); // MOSI
      pinMode(MOSI, OUTPUT);

      STM32.shutdown(msec);
      
      SPI.begin();
    }
};

#endif // _QUANTUM_H

