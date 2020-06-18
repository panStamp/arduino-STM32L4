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
 * Creation date: 29 Apr 2019
 */

#include "panstamp_lorawan.h"


static osjob_t sendjob;

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
//static const u1_t APPEUI[8] = { 0xAE, 0xE7, 0x01, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
uint8_t appEui[LORAWAN_APPEUI_SIZE];
static const u1_t *APPEUI = appEui;
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
uint8_t devEui[LORAWAN_DEVEUI_SIZE];
static const u1_t *DEVEUI = devEui;
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
//static const u1_t APPKEY[16] = { 0x4F, 0x47, 0x0E, 0x26, 0x17, 0x76, 0xEF, 0xCA, 0x5E, 0x3B, 0xFF, 0xB6, 0x06, 0xCC, 0x16, 0x5D };
uint8_t appKey[LORAWAN_APPKEY_SIZE];
static const u1_t *APPKEY = appKey;
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
uint16_t txInterval = 60;

// Pin mapping
const lmic_pinmap lmic_pins = {
  .nss = RF_CS,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = RF_RST,
  .dio = {RF_DIO0, RF_DIO1, LMIC_UNUSED_PIN},
};

// Transmission buffer
uint8_t txBuffer[64];
// Tx buffer length
uint8_t txBufferLen = 0;
// ACK flag
bool askForAck = false;

// Last event detected
ev_t lastEvent = EV_JOINING;

// Pointer to user periodic function
void (*userPeriodicFunct)(osjob_t* j);

// Pointer to user low power sleep function
void (*userSleepFunct)(uint16_t sec);

// Pointer to user downlink function
void (*userDownlinkFunct)(uint8_t * payload, uint16_t length);

/**
 * LMIC do_send function
 */
void do_send(osjob_t* j)
{
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND)
  {
    LORAWAN_DEBUG_PRINTLN(F("OP_TXRXPEND, not sending"));
  }
  else
  {   
    // Set data rate and transmit power for uplink
    LMIC_setDrTxpow(DR_SF7, 14);
  
    // Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1, txBuffer, txBufferLen, (u1_t)askForAck);
    LORAWAN_DEBUG_PRINTLN(F("Packet queued"));
  }
  // Next TX is scheduled after TX_COMPLETE event.
}

/**
 * LMIC onEvent function
 */
void onEvent (ev_t ev)
{
  u4_t netid = 0;
  devaddr_t devaddr = 0;
  u1_t nwkKey[16];
  u1_t artKey[16];
      
  LORAWAN_DEBUG_PRINT(os_getTime());
  LORAWAN_DEBUG_PRINT(": ");
  switch(ev)
  {
    case EV_SCAN_TIMEOUT:
      LORAWAN_DEBUG_PRINTLN(F("EV_SCAN_TIMEOUT"));
      break;
    case EV_BEACON_FOUND:
      LORAWAN_DEBUG_PRINTLN(F("EV_BEACON_FOUND"));
      break;
    case EV_BEACON_MISSED:
      LORAWAN_DEBUG_PRINTLN(F("EV_BEACON_MISSED"));
      break;
    case EV_BEACON_TRACKED:
      LORAWAN_DEBUG_PRINTLN(F("EV_BEACON_TRACKED"));
      break;
    case EV_JOINING:
      LORAWAN_DEBUG_PRINTLN(F("EV_JOINING"));
      break;
    case EV_JOINED:
      LORAWAN_DEBUG_PRINTLN(F("EV_JOINED"));
      LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
      LORAWAN_DEBUG_PRINT("netid: ");
      LORAWAN_DEBUG_PRINTLN(netid, DEC);
      LORAWAN_DEBUG_PRINT("devaddr: ");
      LORAWAN_DEBUG_PRINTLN(devaddr, HEX);
      LORAWAN_DEBUG_PRINT("artKey: ");
      for (int i=0; i<sizeof(artKey); ++i) {
        LORAWAN_DEBUG_PRINT(artKey[i], HEX);
      }
      LORAWAN_DEBUG_PRINTLN("");
      LORAWAN_DEBUG_PRINT("nwkKey: ");
      for (int i=0; i<sizeof(nwkKey); ++i) {
        LORAWAN_DEBUG_PRINT(nwkKey[i], HEX);
      }
      LORAWAN_DEBUG_PRINTLN("");

      // Disable link check validation (automatically enabled
      // during join, but because slow data rates change max TX
      // size, we don't use it in this example.
      LMIC_setLinkCheckMode(0);
      break;
      
    /*
    || This event is defined but not used in the code. No
    || point in wasting codespace on it.
    ||
    || case EV_RFU1:
    ||     LORAWAN_DEBUG_PRINTLN(F("EV_RFU1"));
    ||     break;
    */
    
    case EV_JOIN_FAILED:
      LORAWAN_DEBUG_PRINTLN(F("EV_JOIN_FAILED"));
      break;
    case EV_REJOIN_FAILED:
      LORAWAN_DEBUG_PRINTLN(F("EV_REJOIN_FAILED"));
      break;
    case EV_TXCOMPLETE:
      LORAWAN_DEBUG_PRINTLN(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
      if (LMIC.txrxFlags & TXRX_ACK)
        LORAWAN_DEBUG_PRINTLN(F("Received ack"));
      if (LMIC.dataLen)
      {
        LORAWAN_DEBUG_PRINT(F("Received "));
        LORAWAN_DEBUG_PRINT(LMIC.dataLen);
        LORAWAN_DEBUG_PRINTLN(F(" bytes of payload"));
      }

      digitalWrite(LED_BUILTIN, LOW);

      // Schedule next transmission
      if (userSleepFunct != NULL)
      {
        userSleepFunct(txInterval);

        if (userPeriodicFunct != NULL)
          userPeriodicFunct(&sendjob);
      }
      else if (userPeriodicFunct != NULL)
        os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(txInterval), userPeriodicFunct);
      break;
    case EV_LOST_TSYNC:
      LORAWAN_DEBUG_PRINTLN(F("EV_LOST_TSYNC"));
      break;
    case EV_RESET:
      LORAWAN_DEBUG_PRINTLN(F("EV_RESET"));
      break;
    case EV_RXCOMPLETE:
      // data received in ping slot
      LORAWAN_DEBUG_PRINTLN(F("EV_RXCOMPLETE"));
      break;
    case EV_LINK_DEAD:
      LORAWAN_DEBUG_PRINTLN(F("EV_LINK_DEAD"));
      break;
    case EV_LINK_ALIVE:
      LORAWAN_DEBUG_PRINTLN(F("EV_LINK_ALIVE"));
      break;
    /*
    || This event is defined but not used in the code. No
    || point in wasting codespace on it.
    ||
    || case EV_SCAN_FOUND:
    ||    LORAWAN_DEBUG_PRINTLN(F("EV_SCAN_FOUND"));
    ||    break;
    */
    case EV_TXSTART:
      digitalWrite(LED_BUILTIN, HIGH);
      LORAWAN_DEBUG_PRINTLN(F("EV_TXSTART"));
      break;
    case EV_JOIN_TXCOMPLETE:
      LORAWAN_DEBUG_PRINTLN(F("EV_JOIN_TXCOMPLETE"));
      break;
    default:
      LORAWAN_DEBUG_PRINT(F("Unknown event: "));
      LORAWAN_DEBUG_PRINTLN((unsigned) ev);
      break;
  }

  lastEvent = ev;
}

/**
 * lorawan_transmit
 * 
 * Place buffer in the Tx pool
 * 
 * @param buf New contents to be transmitted
 * @param len Buffer length
 * @param ack Ask for ACK in case of true
 */
void lorawan_transmit(uint8_t *buf, uint8_t len, bool ack)
{
  txBufferLen = len;
  memcpy(txBuffer, buf, len);
  askForAck = ack;
  do_send(&sendjob);
}

/**
 * lorawan_init
 * 
 * Initialize LMIC stack
 *
 * @param adrMode Enable ADR mode if true
 */
void lorawan_init(bool adrMode)
{
  // Initialize user sleep function
  userSleepFunct = NULL;

  // Initialize user periodic function
  userPeriodicFunct = NULL;

  // Initialize user downlink function
  userDownlinkFunct = NULL;

  // LMIC init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();

  // Let LMIC compensate for +/- 1% clock error
  LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);

  // Set ADR mode
  LMIC_setAdrMode(adrMode);
}

/**
 * lorawan_run
 * 
 * Process LMIC stack once
 *
 * @return status related to LMIC stack
 */
PANSTAMP_LORAWAN_STATUS lorawan_run(void)
{
  os_runloop_once();

  if (lastEvent == EV_TXCOMPLETE)
  {
    if (askForAck)
    {
      if (LMIC.txrxFlags & TXRX_ACK)
      {
        LMIC.txrxFlags &= ~TXRX_ACK;
        return PANSTAMP_LORAWAN_STATUS_ACK_RECEIVED;
      }
    }

    if (LMIC.dataLen > 0)
    {
      if (userDownlinkFunct != NULL)
      {
        uint8_t *payload = LMIC.frame + LMIC.dataBeg;
        uint16_t length = LMIC.dataLen;
        LMIC.dataLen = 0;
        userDownlinkFunct(payload, length);        
      }
    }
  }

  return PANSTAMP_LORAWAN_STATUS_NONE;
}

/**
 * lorawan_setTxInterval
 *
 * Set tx interval
 *
 * @param sec New tx interval in seconds
 */
void lorawan_setTxInterval(uint16_t sec)
{
  txInterval = sec;
}

/**
 * lorawan_setUserPeriodicFunct
 *
 * Set user periodic function
 *
 * @param funct pointer to user function
 */
void lorawan_setUserPeriodicFunct(void (*funct)(osjob_t* j))
{
  userPeriodicFunct = funct;
}

/**
 * lorawan_setUserSleepFunct
 *
 * Set user low power sleep function
 *
 * @param funct pointer to user function
 */
void lorawan_setUserSleepFunct(void (*funct)(uint16_t sec))
{
  userSleepFunct = funct;
}

/**
 * lorawan_setUserDownlinkFunct
 *
 * Set user downlink function. To be called whenever a new downlink is received
 *
 * @param funct pointer to user function
 */
void lorawan_setUserDownlinkFunct(void (*funct)(uint8_t * payload, uint16_t length))
{
  userDownlinkFunct = funct;
}

/**
 * lorawan_setDevEui
 *
 * Set LoRaWAN Device EUI (Little Endian)
 *
 * @param eui LoRaWAN Dev EUI
 */
void lorawan_setDevEui(uint8_t *eui)
{
  memcpy(devEui, eui, sizeof(devEui));
}

/**
 * lorawan_setAppKey
 *
 * Set LoRaWAN App key (Big Endian)
 *
 * @param key LoRaWAN App key
 */
void lorawan_setAppKey(uint8_t *key)
{
  memcpy(appKey, key, sizeof(appKey));
}

/**
 * lorawan_setAppEui
 *
 * Set LoRaWAN App EUI (Little Endian)
 *
 * @param eui LoRaWAN App EUI
 */
void lorawan_setAppEui(uint8_t *eui)
{
  memcpy(appEui, eui, sizeof(appEui));
}

/**
 * lorawan_getDevEui
 *
 * Get Device EUI
 *
 * @param deviceEui : device EUI
 */
void lorawan_getDevEui(uint8_t *deviceEui)
{
  memcpy(deviceEui, devEui, sizeof(devEui));
}

/**
 * lorawan_getAppKey
 *
 * Get LoRaWAN Application key
 *
 * @param applicationKey : LoRaWAN application key
 */
void lorawan_getAppKey(uint8_t *applicationKey)
{
  memcpy(applicationKey, appKey, sizeof(appKey));
}

/**
 * lorawan_getAppEui
 *
 * Get LoRaWAN Application EUI via serial
 *
 * @param applicationEui : LoRaWAN application EUI
 */
void lorawan_getAppEui(uint8_t *applicationEui)
{
  memcpy(applicationEui, appEui, sizeof(appEui));
}

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
void lorawan_getSessionKeys(uint32_t *netId, uint32_t *devAddr, uint8_t *nwkSkey, uint8_t *appSkey)
{
  LMIC_getSessionKeys((u4_t*)netId, (devaddr_t*)devAddr, (u1_t*)nwkSkey, (u1_t*)appSkey);
}
