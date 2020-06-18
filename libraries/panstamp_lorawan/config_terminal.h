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
 * Creation date: 28 Apr 2020
 */

#ifndef _CONFIG_TERMINAL_H
#define _CONFIG_TERMINAL_H

#include <EEPROM.h>
#include <Arduino.h>

/**
 * Sizes
 */
#define LORAWAN_DEVEUI_SIZE    8

#define LORAWAN_APPKEY_SIZE    16
#define LORAWAN_APPEUI_SIZE    8
#define LORAWAN_MODE_SIZE      1
#define LORAWAN_NETID_SIZE     4
#define LORAWAN_DEVADDR_SIZE   4
#define LORAWAN_NETSKEY_SIZE   16
#define LORAWAN_APPSKEY_SIZE   16
#define LORAWAN_TXINTER_SIZE   2

#define EEPROM_SIZE            LORAWAN_APPKEY_SIZE + LORAWAN_APPEUI_SIZE + LORAWAN_MODE_SIZE + LORAWAN_NETID_SIZE \
                               + LORAWAN_DEVADDR_SIZE + LORAWAN_NETSKEY_SIZE + LORAWAN_APPSKEY_SIZE + LORAWAN_TXINTER_SIZE
/**
 * EEPROM addresses
 */
#define EEPROM_START_ADDR      0
#define EEPROM_APPKEY_ADDR     EEPROM_START_ADDR
#define EEPROM_APPEUI_ADDR     EEPROM_APPKEY_ADDR + LORAWAN_APPKEY_SIZE
#define EEPROM_MODE_ADDR       EEPROM_APPEUI_ADDR + LORAWAN_APPEUI_SIZE
#define EEPROM_NETID_ADDR      EEPROM_MODE_ADDR + LORAWAN_MODE_SIZE
#define EEPROM_DEVADDR_ADDR    EEPROM_NETID_ADDR + LORAWAN_NETID_SIZE
#define EEPROM_NETSKEY_ADDR    EEPROM_DEVADDR_ADDR + LORAWAN_DEVADDR_SIZE
#define EEPROM_APPSKEY_ADDR    EEPROM_NETSKEY_ADDR + LORAWAN_NETSKEY_SIZE
#define EEPROM_TXINTER_ADDR    EEPROM_APPSKEY_ADDR + LORAWAN_APPSKEY_SIZE
#define EEPROM_USER_ADDR       EEPROM_TXINTER_ADDR + LORAWAN_TXINTER_SIZE

/**
 * Working modes
 */
typedef enum LORAWAN_WORKING_MODE
{
  LORAWAN_WORKING_MODE_OTAA = 0,
  LORAWAN_WORKING_MODE_ABP,
  LORAWAN_WORKING_MODE_MIXED
};

/**
 * lorawan_setTxInterval
 *
 * Set tx interval
 *
 * @param sec New tx interval in seconds
 */
extern void lorawan_setTxInterval(uint16_t sec);

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
 * lorawan_getDevEui
 *
 * Get Device EUI
 *
 * @param deviceEui : device EUI
 */
extern void lorawan_getDevEui(uint8_t *deviceEui);

/**
 * lorawan_getAppKey
 *
 * Get LoRaWAN Application key
 *
 * @param applicationKey : LoRaWAN application key
 */
extern void lorawan_getAppKey(uint8_t *applicationKey);

/**
 * lorawan_getAppEui
 *
 * Get LoRaWAN Application EUI via serial
 *
 * @param applicationEui : LoRaWAN application EUI
 */
extern void lorawan_getAppEui(uint8_t *applicationEui);

/**
 * lorawan_getSessionKeys
 *
 * Retrieve session keys
 *
 * @param netId : network ID
 * @param devAddr : device address
 * @param nwkSkey : network session key
 * @param appSkey : application session key
 */
extern void lorawan_getSessionKeys(uint32_t *netId, uint32_t *devAddr, uint8_t *nwkSkey, uint8_t *appSkey);

/**
 * Config terminal class
 */
class CONFIG_TERMINAL
{
  private:
    /**
     * runSerialCmd
     * 
     * Run serial command
     * 
     * @param cmd : serial command
     */
    inline void runSerialCmd(char *cmd)
    {
      char *ptr = cmd;
      
      if (!strcasecmp(cmd, "show"))
      {
        Serial.print("Tx interval(sec): ");
        Serial.println(readTxIntervalFromEeprom(), DEC);
        Serial.print("Working mode: ");
        printWorkingMode(readModeFromEeprom());
        Serial.println("OTAA parameters:");
        Serial.print("Device EUI : ");
        printDevEui();
        Serial.print("Application key : ");
        printAppKey();
        Serial.print("Application EUI : ");
        printAppEui();
        Serial.println("Session keys:");
        printSessionKeys();
        Serial.println();
      }
      else if (!strcasecmp(cmd, "restart"))
      {
        Serial.print("Restarting device in 5 seconds...");
        delay(5000);
        STM32.reset();
      }
      else if (!strcasecmp(cmd, "reset config"))
      {
        resetConfig();
        Serial.print("Config in EEPROM set to factory defaults");
      }
      else if (!strcasecmp(cmd, "show eeprom"))
      {
        Serial.println("EEPROM contents:");
        printEepromContents();
      }
      else if (!strcasecmp(cmd, "save session"))
      {
        if (saveSessionKeysInEeprom())
          Serial.print("Current session keys saved in EEPROM");
        else
          Serial.println("Connection not established yet. Current session can not be saved");
      }
      else if (!strncasecmp(cmd, "set txinterval ", 15))
      {
        ptr += 15;       
        saveTxIntervalInEeprom(ptr);
        Serial.print("New Tx interval saved: ");
        Serial.println(ptr);
        Serial.println("Remember to restart device");
      }
      else if (!strncasecmp(cmd, "set mode ", 9))
      {
        ptr += 9;       
        saveModeInEeprom(ptr);
        Serial.print("New working mode saved: ");
        Serial.println(ptr);
        Serial.println("Remember to restart device");
      }   
      else if (!strncasecmp(cmd, "set appkey ", 11))
      {
        ptr += 11;

        if (strlen(ptr) != 32)
        {
          Serial.println("Incorrect Application Key format");
          return;
        }
        
        saveAppKeyInEeprom(ptr);
        Serial.print("New Application Key saved: ");
        Serial.println(ptr);
        Serial.println("Remember to restart device");
      }
      else if (!strncasecmp(cmd, "set appeui ", 11))
      {
        ptr += 11;

        if (strlen(ptr) != 16)
        {
          Serial.println("Incorrect Application EUI format");
          return;
        }
          
        saveAppEuiInEeprom(ptr);
        Serial.print("New application EUI saved: ");
        Serial.println(ptr);
        Serial.println("Remember to restart device");
      }
      else
        printMenu();
    }


    /**
     * resetConfig
     *
     * Reset configuration in "EEPROM" to factory defaults (all 0xFF's)
     */
    inline void resetConfig(void)
    {
      for(int i=0 ; i<EEPROM_SIZE ; i++)
        EEPROM.write(EEPROM_START_ADDR + i, 0xFF);
    }

    /**
     * saveAppKeyInEeprom
     *
     * Save App key in "EEPROM"
     *
     * @param key LoRaWAN App key in char string format
     */
    inline void saveAppKeyInEeprom(char *key)
    {
      char strByte[3];
      uint8_t b;

      for(int i=0 ; i<LORAWAN_APPKEY_SIZE ; i++)
      {
        strByte[0] = key[2*i];
        strByte[1] = key[2*i + 1];
        strByte[2] = 0;
        b = strtol(strByte, NULL, 16);
        EEPROM.write(EEPROM_APPKEY_ADDR + i, b);
      }
    }

    /**
     * saveAppEuiInEeprom
     *
     * Save App EUI in "EEPROM"
     *
     * @param key LoRaWAN App EUI in char string format
     */
    inline void saveAppEuiInEeprom(char *eui)
    {
      char strByte[3];
      uint8_t b;

      for(int i=0 ; i<LORAWAN_APPEUI_SIZE ; i++)
      {
        strByte[0] = eui[2*i];
        strByte[1] = eui[2*i + 1];
        strByte[2] = 0;
        b = strtol(strByte, NULL, 16);
        EEPROM.write(EEPROM_APPEUI_ADDR + i, b);
      }
    }

    /**
     * saveModeInEeprom
     *
     * Save working mode in "EEPROM"
     *
     * @param mode : working mode ("OTAA", "ABP", "mixed")
     */
    inline void saveModeInEeprom(char *mode)
    {
      LORAWAN_WORKING_MODE val = LORAWAN_WORKING_MODE_OTAA; // OTAA by default

      if (!strcasecmp(mode, "abp"))
        val = LORAWAN_WORKING_MODE_ABP;
      else if (!strcasecmp(mode, "mixed"))
        val = LORAWAN_WORKING_MODE_MIXED;

      EEPROM.write(EEPROM_MODE_ADDR, val);
    }

    /**
     * saveTxIntervalInEeprom
     *
     * Save transmission interval in "EEPROM"
     *
     * @param interval : transmission interval (in sec) in string format
     *
     * @return true in case of tx interval successfully saved in EEPROM space
     */
    inline bool saveTxIntervalInEeprom(char *interval)
    {
      uint16_t txInterval = (uint16_t) atoi(interval);

      if (txInterval == 0)
        return false;

      // Save device address
      for(int i=0 ; i<LORAWAN_TXINTER_SIZE ; i++)
        EEPROM.write(EEPROM_TXINTER_ADDR + i, (txInterval >> 8*(LORAWAN_TXINTER_SIZE-1-i)) & 0xFF);

      return true;
    }

    /**
     * saveSessionKeysInEeprom
     *
     * Save LoRaWAN session keys in "EEPROM"
     *
     * @return true in case of session successfully saved in EEPROM space
     */
    inline bool saveSessionKeysInEeprom(void)
    {
      uint32_t netId;
      uint32_t devAddr;
      uint8_t nwkSkey[LORAWAN_NETSKEY_SIZE];
      uint8_t appSkey[LORAWAN_APPSKEY_SIZE];

      // Retrieve session keys
      lorawan_getSessionKeys(&netId, &devAddr, nwkSkey, appSkey);

      if (devAddr == 0)
        return false;

      // Save network ID
      for(int i=0 ; i<LORAWAN_NETID_SIZE ; i++)
        EEPROM.write(EEPROM_NETID_ADDR + i, (netId >> 8*(LORAWAN_NETID_SIZE-1-i)) & 0xFF);

      // Save device address
      for(int i=0 ; i<LORAWAN_DEVADDR_SIZE ; i++)
        EEPROM.write(EEPROM_DEVADDR_ADDR + i, (devAddr >> 8*(LORAWAN_DEVADDR_SIZE-1-i)) & 0xFF);

      // Save application session key
      for(int i=0 ; i<LORAWAN_APPSKEY_SIZE ; i++)
        EEPROM.write(EEPROM_APPSKEY_ADDR + i, appSkey[i]);

      // Save network session key
      for(int i=0 ; i<LORAWAN_NETSKEY_SIZE ; i++)
        EEPROM.write(EEPROM_NETSKEY_ADDR + i, nwkSkey[i]);

      return true;
    }

    /**
     * printEepromContents
     *
     * Print whole EEPROM space contents
     */
    void printEepromContents(void)
    {
      for(uint16_t i=0 ; i<EEPROM_SIZE ; i++)
      {
        Serial.print(EEPROM.read(EEPROM_START_ADDR + i), HEX);
        Serial.print(" ");
      }
      Serial.println();
    }

    /**
     * printDevEui
     *
     * Print Device EUI via serial
     */
    void printDevEui(void)
    {
      uint8_t devEui[LORAWAN_DEVEUI_SIZE];
      lorawan_getDevEui(devEui);

      char eui[20];
      sprintf(eui, "%02X%02X%02X%02X%02X%02X%02X%02X",
      devEui[0], devEui[1], devEui[2], devEui[3], devEui[4], devEui[5], devEui[6], devEui[7]);

      Serial.println(eui);
    }

    /**
     * printAppKey
     *
     * Print App key via serial
     */
    void printAppKey(void)
    {
      uint8_t appKey[LORAWAN_APPKEY_SIZE];
      lorawan_getAppKey(appKey);

      char key[40];
      sprintf(key, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
      appKey[0], appKey[1], appKey[2], appKey[3], appKey[4], appKey[5], appKey[6], appKey[7],
      appKey[8], appKey[9], appKey[10], appKey[11], appKey[12], appKey[13], appKey[14], appKey[15]);

      Serial.println(key);
    }

    /**
     * printAppEui
     *
     * Print App EUI via serial
     */
    void printAppEui(void)
    {
      uint8_t appEui[LORAWAN_APPEUI_SIZE];
      lorawan_getAppEui(appEui);

      char eui[20];
      sprintf(eui, "%02X%02X%02X%02X%02X%02X%02X%02X",
      appEui[0], appEui[1], appEui[2], appEui[3], appEui[4], appEui[5], appEui[6], appEui[7]);

      Serial.println(eui);
    }

    /**
     * printSessionKeys
     *
     * Print session keys via serial
     */
    inline void printSessionKeys(void)
    {
      uint32_t netId;
      uint32_t devAddr;
      uint8_t nwkSkey[LORAWAN_NETSKEY_SIZE];
      uint8_t appSkey[LORAWAN_APPSKEY_SIZE];

      lorawan_getSessionKeys(&netId, &devAddr, nwkSkey, appSkey);

      if (devAddr == 0)
        Serial.println("Connection not established yet");
      else
      {
        Serial.print("Network ID: ");
        Serial.println(netId, DEC);
        Serial.print("Device address: ");
        Serial.println(devAddr, HEX);

        char buf[40];
        Serial.print("Network Session Key: ");
        sprintf(buf, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
        nwkSkey[0], nwkSkey[1], nwkSkey[2], nwkSkey[3], nwkSkey[4], nwkSkey[5], nwkSkey[6], nwkSkey[7],
        nwkSkey[8], nwkSkey[9], nwkSkey[10], nwkSkey[11], nwkSkey[12], nwkSkey[13], nwkSkey[14], nwkSkey[15]);
        Serial.println(buf);

        Serial.print("Application Session Key: ");
        sprintf(buf, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
        appSkey[0], appSkey[1], appSkey[2], appSkey[3], appSkey[4], appSkey[5], appSkey[6], appSkey[7],
        appSkey[8], appSkey[9], appSkey[10], appSkey[11], appSkey[12], appSkey[13], appSkey[14], appSkey[15]);
        Serial.println(buf);
      }
    }

    /**
     * printWorkingMode
     *
     * Print working mode in string format
     *
     * @param mode : working mode in LORAWAN_WORKING_MODE format
     */
    inline void printWorkingMode(LORAWAN_WORKING_MODE mode)
    {
      switch(mode)
      {
        case LORAWAN_WORKING_MODE_OTAA:
          Serial.println("OTAA");
          break;
        case LORAWAN_WORKING_MODE_ABP:
          Serial.println("ABP");
          break;
        case LORAWAN_WORKING_MODE_MIXED:
          Serial.println("Mixed");
          break;
        default:
          Serial.println("Unknown");
          break;
      }
    }

  public:  

    /**
     * begin
     *
     * Initialize serial config terminal
     */
    inline void begin(void)
    {
      Serial.begin(115200);
    }

    /**
     * end
     *
     * End serial config terminal
     */
    inline void end(void)
    {
      Serial.end();
    }

    /**
     * printMenu
     *
     * Print user menu via serial
     */
    inline void printMenu(void)
    {
      Serial.println();
      Serial.println("show           - Show LoRaWAN parameters");
      Serial.println("restart        - Restart device");
      Serial.println("reset config   - Reset configuration");
      Serial.println("set txinterval - Set transmission interval in sec (ex: set txinterval 60)");
      Serial.println("set mode       - Set working mode (ex: set mode [otaa, abp, mixed])");
      Serial.println("set appkey     - Set LoRaWAN application key (ex: set appkey 12457896219824001245789621982400)");
      Serial.println("set appeui     - Set LoRaWAN application EUI (ex: set appeui 1245789621982400)");
      Serial.println("save session   - Save LoRaWAN session keys");
      Serial.println();
    }

    /**
     * readAppKeyFromEeprom
     *
     * Read App key from "EEPROM"
     *
     * @param key LoRaWAN App key in byte array format
     *
     * @return true if result is different than all 0xFF's
     */
    inline bool readAppKeyFromEeprom(uint8_t *key)
    {
      bool res = false;

      for(uint8_t i=0 ; i<LORAWAN_APPKEY_SIZE ; i++)
      {
        key[i] = EEPROM.read(EEPROM_APPKEY_ADDR + i);
        if (key[i] != 0xFF)
          res = true;
      }
      return res;
    }

    /**
     * readAppEuiFromEeprom
     *
     * Read App EUI from "EEPROM"
     *
     * @param eui LoRaWAN App EUI in byte array format
     *
     * @return true if result is different than all 0xFF's
     */
    inline bool readAppEuiFromEeprom(uint8_t *eui)
    {
      bool res = false;

      for(uint8_t i=0 ; i<LORAWAN_APPEUI_SIZE ; i++)
      {
        eui[i] = EEPROM.read(EEPROM_APPEUI_ADDR + i);
        if (eui[i] != 0xFF)
          res = true;
      }
      return res;
    }

    /**
     * readModeFromEeprom
     *
     * Read working mode from "EEPROM"
     *
     * @return working mode
     */
    inline LORAWAN_WORKING_MODE readModeFromEeprom(void)
    {
      return (LORAWAN_WORKING_MODE) EEPROM.read(EEPROM_MODE_ADDR);
    }

    /**
     * readTxIntervalFromEeprom
     *
     * Read transmission interval from "EEPROM"
     *
     * @return transmission interval (in sec) read from EEPROM space
     */
    inline uint16_t readTxIntervalFromEeprom(void)
    {
      uint16_t txInterval = 0;

      // Save device address
      for(int i=0 ; i<LORAWAN_TXINTER_SIZE ; i++)
      {
        txInterval <<= 8;
        txInterval |= EEPROM.read(EEPROM_TXINTER_ADDR + i);
      }

      return txInterval;
    }

    /**
     * readSessionKeysFromEeprom
     *
     * Read LoRaWAN session keys from EEPROM
     *
     * @param netId : network ID
     * @param devAddr : device address
     * @param nwkSkey : network session key
     * @param appSkey : application session key
     *
     * @return true if result is different than all 0xFF's
     */
    inline bool readSessionKeysFromEeprom(uint32_t *netId, uint32_t *devAddr, uint8_t *nwkSkey, uint8_t *appSkey)
    {
      uint32_t addr = 0, net = 0;

      // Read device address from EEPROM space
      for(uint8_t i=0 ; i<LORAWAN_DEVADDR_SIZE ; i++)
      {
        addr <<= 8;
        addr |= EEPROM.read(EEPROM_DEVADDR_ADDR + i);
      }

      if (addr == 0)
        return false;

      *devAddr = addr;

      // Read network ID from EEPROM space
      for(uint8_t i=0 ; i<LORAWAN_NETID_SIZE ; i++)
      {
        net <<= 8;
        net |= EEPROM.read(EEPROM_NETID_ADDR + i);
      }

      *netId = net;

      // Read application session key from EEPROM space
      for(uint8_t i=0 ; i<LORAWAN_APPSKEY_SIZE ; i++)
        appSkey[i] = EEPROM.read(EEPROM_APPSKEY_ADDR + i);

      // Read network session key from EEPROM space
      for(uint8_t i=0 ; i<LORAWAN_NETSKEY_SIZE ; i++)
        nwkSkey[i] = EEPROM.read(EEPROM_NETSKEY_ADDR + i);

      return true;
    }

    /**
     * saveTxIntervalInEeprom
     *
     * Save transmission interval in "EEPROM"
     *
     * @param interval : transmission interval (in sec)
     *
     * @return true in case of tx interval successfully saved in EEPROM space
     */
    inline bool saveTxIntervalInEeprom(uint16_t interval)
    {
      if (interval == 0)
        return false;

      // Save device address
      for(int i=0 ; i<LORAWAN_TXINTER_SIZE ; i++)
        EEPROM.write(EEPROM_TXINTER_ADDR + i, (interval >> 8*(LORAWAN_TXINTER_SIZE-1-i)) & 0xFF);

      return true;
    }
    
    /**
     * processSerial
     * 
     * Process serial comms from terminal
     */
    inline void processSerial(void)
    {
      static char rxBuffer[64] = "";
      static uint8_t rxLength = 0;

      while(Serial.available())
      {
        char ch = Serial.read();

        if ((ch == '\r') || (ch == '\n'))
        {
          rxBuffer[rxLength] = 0;
          runSerialCmd(rxBuffer);
          rxLength = 0;
          // Flush Rx
          while(Serial.available()) Serial.read();
        }
        else
          rxBuffer[rxLength++] = ch;
      }
    }
};

#endif

