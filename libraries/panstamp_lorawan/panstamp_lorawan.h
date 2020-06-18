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

#ifndef _PANSTAMP_LORAWAN_H
#define _PANSTAMP_LORAWAN_H

#include "config_terminal.h"

#include <lmic.h>
#include <hal/hal.h>
#include <Arduino.h>

/**
 * Macros
 */
//#define LORAWAN_SERIAL_DEBUG  1
#ifdef LORAWAN_SERIAL_DEBUG
  #define LORAWAN_DEBUG_BEGIN(...) Serial.begin(__VA_ARGS__); Serial.flush()
  #define LORAWAN_DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
  #define LORAWAN_DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
  #define LORAWAN_DEBUG_BEGIN(...)
  #define LORAWAN_DEBUG_PRINT(...)
  #define LORAWAN_DEBUG_PRINTLN(...)
#endif

/**
 * Status related to LMIC stack
 */
enum PANSTAMP_LORAWAN_STATUS
{
  PANSTAMP_LORAWAN_STATUS_NONE = 0,
  PANSTAMP_LORAWAN_STATUS_ACK_RECEIVED
};

/**
 * lorawan_init
 * 
 * Initialize LMIC stack
 *
 * @param adrMode Enable ADR mode if true
 */
extern void lorawan_init(bool adrMode);

/**
 * lorawan_transmit
 * 
 * Place buffer in the Tx pool
 * 
 * @param buf New contents to be transmitted
 * @param len Buffer length
 * @param ack Ask for ACK in case of true
 */
void lorawan_transmit(uint8_t *buf, uint8_t len, bool ack=false);

/**
 * lorawan_run
 * 
 * Process LMIC stack once
 *
 * @return status related to LMIC stack
 */
extern PANSTAMP_LORAWAN_STATUS lorawan_run(void);

/**
 * lorawan_setTxInterval
 *
 * Set tx interval
 *
 * @param sec New tx interval in seconds
 */
extern void lorawan_setTxInterval(uint16_t sec);

/**
 * lorawan_setUserPeriodicFunct
 *
 * Set user periodic function
 *
 * @param funct pointer to user function
 */
extern void lorawan_setUserPeriodicFunct(void (*funct)(osjob_t* j));

/**
 * lorawan_setUserSleepFunct
 *
 * Set user low power sleep function
 *
 * @param funct pointer to user function
 */
extern void lorawan_setUserSleepFunct(void (*funct)(uint16_t sec));

/**
 * lorawan_setUserDownlinkFunct
 *
 * Set user downlink function. To be called whenever a new downlink is received
 *
 * @param funct pointer to user function
 */
extern void lorawan_setUserDownlinkFunct(void (*funct)(uint8_t * payload, uint16_t length));

/**
 * lorawan_setDevEui
 *
 * Set LoRaWAN Device EUI (Little Endian)
 *
 * @param eui LoRaWAN Dev EUI
 */
extern void lorawan_setDevEui(uint8_t *eui);

/**
 * lorawan_setAppKey
 *
 * Set LoRaWAN App key
 *
 * @param key LoRaWAN App key
 */
extern void lorawan_setAppKey(uint8_t *key);

/**
 * lorawan_setAppEui
 *
 * Set LoRaWAN App EUI (Little Endian)
 *
 * @param eui LoRaWAN App EUI
 */
extern void lorawan_setAppEui(uint8_t *eui);


/**
 * C++ wrapping class
 */
class PANSTAMP_LORAWAN
{
  private:
    /**
     * Enable serial config
     */
    bool enableSerialConfig;

    /**
     * Serial config interface
     */
    CONFIG_TERMINAL config;

    /**
     * downlinksEnabled
     *
     * Downlinks enabled if true
     */
    bool downlinksEnabled;

    /**
     * Tx interval
     */
    uint16_t txInterval;

    /**
     * WDT enable flag
     */
    bool wdtEnabled;

    /**
     * txCounter
     *
     * Counts transmissions
     */
    uint16_t txCounter;

    /**
     * requestAck
     *
     * request ACK on next transmission
     */
    bool requestAck;

    /**
     * ackExpectedNb
     *
     * Number of expected ACK's
     */
    uint8_t ackExpectedNb;

    /**
     * ackActivationPeriod
     *
     * Transmissions elapsed between ACK requests
     */
    uint16_t ackActivationPeriod;

    /**
     * ackFailureLimit
     *
     * Maximum amount of ACK failures (no receptions) allowed
     */
    uint8_t ackFailureLimit;

  public:  

    /**
     * Class constructor
     */
    inline PANSTAMP_LORAWAN(void) {}

    /**
     * Class constructor
     *
     * @param interval Tx interval
     * @param downLinksEn Downlinks enable flag
     */
    inline PANSTAMP_LORAWAN(const uint16_t interval, bool downLinksEn=true)
    {
      enableSerialConfig = false;
      downlinksEnabled = downLinksEn;
      txInterval = interval;
      wdtEnabled = false;
      ackExpectedNb = 0;
      ackActivationPeriod = 0;
      ackFailureLimit = 0;
      txCounter = 0;
    }

    /**
     * setSerialConfig
     *
     * Enable or disable serial config engine
     *
     * @param state : serial config state
     */
    inline void setSerialConfig(bool state)
    {
      enableSerialConfig = state;

      if (state)
        config.begin();
      else
        config.end();
    }

    /**
     * begin
     *
     * Initialize LoRaWAN LMIC stack
     *
     * @param adrMode Enable ADR mode if true
     */
    inline void begin(bool adrMode=false)
    {
      LORAWAN_DEBUG_BEGIN(115200);

      // Read working mode from EEPROM space
      uint8_t mode = config.readModeFromEeprom();

      // Read Tx interval from EEPROM space
      uint16_t interval = config.readTxIntervalFromEeprom();

      // Valid Tx interval read from EEPROM?
      if ((interval > 0) && (interval < 0xFF))
        txInterval = interval;

      // Read App Key from EEPROM space
      uint8_t appKey[LORAWAN_APPKEY_SIZE];
      if (config.readAppKeyFromEeprom(appKey))
        lorawan_setAppKey(appKey);

      // Read App EUI from EEPROM space
      uint8_t appEui[LORAWAN_APPEUI_SIZE];
      if (config.readAppEuiFromEeprom(appEui))
        lorawan_setAppEui(appEui);

      // Initialize LoRaWAN stack
      lorawan_init(adrMode);
      
      // Set downlinks
      setDownlinks(downlinksEnabled);
      
      // Working mode other than OTAA?
      if ((mode == LORAWAN_WORKING_MODE_ABP) || (mode == LORAWAN_WORKING_MODE_MIXED))
      {
        uint32_t netId;
        uint32_t devAddr;
        uint8_t nwkSkey[LORAWAN_NETSKEY_SIZE];
        uint8_t appSkey[LORAWAN_APPSKEY_SIZE];

        // Read session keys from config
        if (config.readSessionKeysFromEeprom(&netId, &devAddr, nwkSkey, appSkey))
        { 
          // Set ABP session keys         
          LMIC_setSession (netId, devAddr, nwkSkey, appSkey);
          // Disable link check validation
          LMIC_setLinkCheckMode(0);
        }
      }

      // Set Tx interval
      setTxInterval(txInterval);
    }

    /**
     * setDefaultAppKey
     * 
     * Set default App key
     * 
     * @param appKey : default App key
     */
    inline void setDefaultAppKey(uint8_t *appKey)
    {
      lorawan_setAppKey(appKey);
    }

    /**
     * setDefaultAppEui
     * 
     * Set default App EUI
     * 
     * @param appEui : default App EUI
     */
    inline void setDefaultAppEui(uint8_t *appEui)
    {
      lorawan_setAppEui(appEui);
    } 

    /**
     * run
     *
     * Process LMIC stack
     *
     * @return Status related to the LMIC stack
     */
    inline PANSTAMP_LORAWAN_STATUS run(void)
    {
      if (wdtEnabled)
      {
        STM32.wdtReset();
      }

      if ((ackFailureLimit > 0) && (ackActivationPeriod > 0) && (txCounter >= ackActivationPeriod))
      {
        requestAck = true;

        if (ackExpectedNb >= ackFailureLimit)
          STM32.reset();
      }

      PANSTAMP_LORAWAN_STATUS res = lorawan_run();

      if (res == PANSTAMP_LORAWAN_STATUS_ACK_RECEIVED)
      {
        requestAck = false;
        txCounter = 0;
        ackExpectedNb = 0;
        setDownlinks(downlinksEnabled);
      }

      if (enableSerialConfig)
        config.processSerial();

      return res;
    }

    /**
     * wdtEnable
     *
     * Enable watchdog timer
     *
     * @param timeout Timeout in msec. Max 32000 msec.
     * @param ackPeriod Transmissions elapsed between ACK requests
     @ @param ackLimit Maximum amount of ACK failures (no receptions) allowed
     */
    inline void wdtEnable(uint32_t timeout, uint16_t ackPeriod=0, uint8_t ackLimit=0)
    {
      if (timeout > 0)
      {
        wdtEnabled = true;
        STM32.wdtEnable(timeout);
        STM32.wdtReset();
      }

      ackActivationPeriod = ackPeriod;
      ackFailureLimit = ackLimit;
    }

    /**
     * transmit
     *
     * Place buffer in the Tx pool
     * 
     * @param buf New contents to be transmitted
     * @param len Buffer length
     * @param ack Ask for ACK in case of true
     */
    inline void transmit(uint8_t *buf, uint8_t len, bool ack=false)
    {
      txCounter++;

      if (requestAck || ack)
      {
        ack = true;
        if (!downlinksEnabled)
        {
          setDownlinks(true);
          downlinksEnabled = false;
        }
        ackExpectedNb++;
      }

      lorawan_transmit(buf, len, ack);
    }

    /**
     * getTxInterval
     *
     * Get tx interval
     *
     * @return TX interval in seconds
     */
    inline uint16_t getTxInterval(void)
    {
      return txInterval;
    }

    /**
     * setTxInterval
     *
     * Set tx interval
     *
     * @param sec : new tx interval in seconds
     * @param save : save parameter in EEPROM if this argument is true
     */
    inline void setTxInterval(uint16_t sec, bool save = false)
    {
      txInterval = sec;
      lorawan_setTxInterval(sec);

      if (save)
        config.saveTxIntervalInEeprom(txInterval);
    }

    /**
     * setDevEui
     *
     * Set LoRaWAN Device EUI (Little Endian)
     *
     * @param eui LoRaWAN Dev EUI
     */
    inline void setDevEui(uint8_t *eui)
    {
      lorawan_setDevEui(eui);
    }

    /**
     * setUserPeriodicFunction
     *
     * Set user periodic function
     *
     * @param funct pointer to user function
     */
    inline void setUserPeriodicFunction(void (*funct)(osjob_t* j))
    {
      lorawan_setUserPeriodicFunct(funct);
    }

    /**
     * setUserSleepFunct
     *
     * Set user low power sleep function
     *
     * @param funct pointer to user function
     */
    inline void setUserSleepFunct(void (*funct)(uint16_t sec))
    {
      lorawan_setUserSleepFunct(funct);
    }

  /**
   * setUserDownlinkFunct
   *
   * Set user downlink function. To be called whenever a new downlink is received
   *
   * @param funct pointer to user function
   */
    inline void setUserDownlinkFunct(void (*funct)(uint8_t * payload, uint16_t length))
    {
      lorawan_setUserDownlinkFunct(funct);
    }

    /**
     * setDownlinks
     *
     * Enable or disable downlinks
     *
     * @param enable Enable downlinks if True.
     */
    inline void setDownlinks(bool enable)
    {
      downlinksEnabled = enable;
      LMIC_setDownlinks(enable);
    }
};

#endif

