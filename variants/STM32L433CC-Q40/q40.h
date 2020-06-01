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
 * Creation date: 27 Jul 2020
 */

#ifndef _Q40_H
#define _Q40_H

#include "quantum.h"
#include "quantum_backup.h"
#include "quantum_counter_input.h"
#include "quantum_binary_input.h"
#include "quantum_current_input.h"
#include "quantum_binary_output.h"
#include <thermistor.h>

/**
 * Q4.0 event codes
 */
enum Q40_EVENT_CODE
{
  Q40_EVENT_NONE = 0,
  Q40_EVENT_BINARY,
  Q40_EVENT_COUNTER
};

/**
 * Type definition : Pointer to sleep function
 */
typedef void (*sleepFctn)(uint16_t sec);

/**
 * Main class
 */
class Q40
{
  private:

    /**
     * QUANTUM object
     */
    QUANTUM quantum;

    /**
     * Onboard NTC thermistor
     */
    THERMISTOR ntc;

    /**
     * Backup space
     */
    QUANTUM_BACKUP backupSpace;

  public:

    /**
     * Binary input states
     */
    uint8_t binInputStates;

    /**
     * Binary output states
     */
    uint8_t binOutputStates;

    /**
     * Counting pins
     */
    QUANTUM_COUNTER_INPUT *counterInputs[Q40_NUMBER_BINARY_INPUTS];

    /**
     * Number of counter pins
     */
    uint8_t nbOfCounterPins;

    /**
     * Binary input pins
     */
    QUANTUM_BINARY_INPUT *binaryInputs[Q40_NUMBER_BINARY_INPUTS];

    /**
     * Number of regular binary pins
     */
    uint8_t nbOfBinaryPins;

    /**
     * Binary output pins
     */
    QUANTUM_BINARY_OUTPUT *binaryOutputs[Q40_NUMBER_BINARY_INPUTS];

    /**
     * 4-20mA current input pins
     */
    QUANTUM_CURRENT_INPUT *currentInputs[Q40_NUMBER_CURRENT_INPUTS];

    /**
     * Class constructor
     */
    inline Q40(void) : ntc(NTC_PIN, 10000, 3950, 10000)
    {
      binInputStates = 0;
      nbOfCounterPins = 0;
      nbOfBinaryPins = 0;
      binOutputStates = 0;

      // Current inputs
      static QUANTUM_CURRENT_INPUT currentInput1(ADCINP1);
      static QUANTUM_CURRENT_INPUT currentInput2(ADCINP2);
      currentInputs[0] = &currentInput1;
      currentInputs[1] = &currentInput2;

      // Binary outputs
      static QUANTUM_BINARY_OUTPUT output1(BINOUT1);
      static QUANTUM_BINARY_OUTPUT output2(BINOUT2);
      binaryOutputs[0] = &output1;
      binaryOutputs[1] = &output2;
    }

    /**
     * begin
     *
     * Initialize Q4.0 board
     */
    inline void begin(void)
    {
      // Init quantum core
      quantum.begin();

      // Init backup space
      backupSpace.begin();

      // Init regular binary pins
      binInputStates = 0;
      for (uint8_t i=0 ; i<nbOfBinaryPins ; i++)
      {
        binaryInputs[i]->begin();

        if (binaryInputs[i]->state)
          binInputStates |= 1 << i;
        else
          binInputStates &= ~(1 << i);
      }

      // Init counter pin states
      for (uint8_t i=0 ; i<nbOfCounterPins ; i++)
      {
        counterInputs[i]->begin();

        if (counterInputs[i]->state)
          binInputStates |= 1 << i;
        else
          binInputStates &= ~(1 << i);
      }

      // Init binary outputs
      for (uint8_t i=0 ; i<Q40_NUMBER_BINARY_OUTPUTS ; i++)
      {
        binaryOutputs[i]->begin();

        if (binaryOutputs[i]->state)
          binOutputStates |= 1 << i;
        else
          binOutputStates &= ~(1 << i);
      }
    }

    /**
     * getDevEui
     *
     * Get pointer to 8-byte device EUI
     *
     * @return pointer to Dev EUI
     */
    inline uint8_t *getDevEui(void)
    {
      return quantum.devEui;
    }

    /**
     * getStopFctn
     *
     * Get pointer to low-power stop function
     *
     * @return pointer to stop function
     */
    inline sleepFctn getStopFctn(void)
    {
      return quantum.stopSec;
    }

    /**
     * getStandbyFctn
     *
     * Get pointer to low-power standby function
     *
     * @return pointer to standby function
     */
    inline sleepFctn getStandbyFctn(void)
    {
      return quantum.standbySec;
    }

    /**
     * getShutdownFctn
     *
     * Get pointer to low-power shutdown function
     *
     * @return pointer to shutdown function
     */
    inline sleepFctn getShutdownFctn(void)
    {
      return quantum.shutdownSec;
    }

    /**
     * addCounterInput
     *
     * Add counter input pin
     *
     * @param cPin Pointer to new counter pin
     */
    inline void addCounterInput(QUANTUM_COUNTER_INPUT *cPin)
    {
      counterInputs[nbOfCounterPins++] = cPin;
    }

    /**
     * addBinaryInput
     *
     * Add regular binary input
     *
     * @param bPin Pointer to new binary pin
     */
    inline void addBinaryInput(QUANTUM_BINARY_INPUT *bPin)
    {
      binaryInputs[nbOfBinaryPins++] = bPin;
    }

    /**
     * readBinInputs
     *
     * Read binary inputs
     *
     * @return Event code (Q40_EVENT_CODE)
     */
    inline uint8_t readBinInputs(void)
    {
      bool state;
      uint8_t event = Q40_EVENT_NONE;

      for(uint8_t i=0 ; i < nbOfBinaryPins ; i++)
      {
        if (binaryInputs[i]->update())
        {
          event = Q40_EVENT_BINARY;

          if (binaryInputs[i]->state)
            binInputStates |= 1 << i;
          else
            binInputStates &= ~(1 << i);
        }
      }

      for(uint8_t i=0 ; i < nbOfCounterPins ; i++)
      {
        if (counterInputs[i]->update())
          event = Q40_EVENT_COUNTER;
      }

      return event;
    }

    /**
     * readTemperature
     *
     * Read on-board NTC
     *
     * @return Temperature in Celsius degrees
     */
    inline float readTemperature(void)
    {
      return ntc.read();
    }

    /**
     * run
     *
     * Run loop once
     *
     * @return Event code (Q40_EVENT_CODE)
     */
    inline uint8_t run(void)
    {
      return readBinInputs();
    }
};

#endif

