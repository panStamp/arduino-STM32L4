/*
 * Copyright (c) 2016 Thomas Roell.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal with the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimers.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimers in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Thomas Roell, nor the names of its contributors
 *     may be used to endorse or promote products derived from this Software
 *     without specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * WITH THE SOFTWARE.
 */

#ifndef _VARIANT_QUANTUM_STM32L432KC_
#define _VARIANT_QUANTUM_STM32L432KC_

// The definitions here needs a STM32L4 core >=1.6.6
#define ARDUINO_STM32L4_VARIANT_COMPLIANCE 10606

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

#define STM32L4_CONFIG_LSECLK             32768
#define STM32L4_CONFIG_HSECLK             0
#define STM32L4_CONFIG_SYSOPT             0
#define STM32L4_CONFIG_USB_VBUS           GPIO_PIN_PA15

#define USBCON

/** Master clock frequency */
#define VARIANT_MCK			  F_CPU

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
#include "USBAPI.h"
#include "Uart.h"
#endif // __cplusplus

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

/*
                           ________
      GND                 1|      |16               GND
      D11 MOSI    (PA7)   2|      |17 (PA8)  RF_CS  D10
      D12 MISO    (PA6)   3|      |18 (PA2)  DIO1   D9
      D13 SCK     (PA1)   4|      |19 (PA3)  DIO0   D8
      D14 A0 DAC0 (PA4)   5|      |20 (PB1)  RF_RST D18
      D15 A1 DAC1 (PA5)   6|      |21 (PB7)  SDA    D20
      D16 A2      (PA0)   7|      |22 (PB6)  SCL    D21
      D17 A3      (PB0)   8|      |23 (PB5)  LED    D5
      D1  TXD1    (PA9)   9|      |24 (PB4)         D4
      D0  RXD1    (PA10) 10|      |25 (PB3)         D3
      GND                11|      |26 (PA14) SWDCLK D22
      VCC                12|      |27 (PA13) SWDIO  D23
      GND                13|      |28               RESET
      +BAT               14|      |29               GND
      +5V                15|______|30               VCC

*/

// Number of pins defined in PinDescription array
#define PINS_COUNT           (25u)
#define NUM_DIGITAL_PINS     (19u)
#define NUM_TOTAL_PINS       (42u)
#define NUM_ANALOG_INPUTS    (5u)
#define NUM_ANALOG_OUTPUTS   (2u)
#define analogInputToDigitalPin(p)  ((p < 6u) ? (p) + 14u : -1)

// SX1276 pins
#define RF_DIO0              (8u)
#define RF_DIO1              (9u)
#define RF_CS                (10u)
#define RF_RST               (18u)

// MMA8652FC pins
#define MMA_EN               (4u)
#define MMA_INT              (16u)

// LEDs
#define PIN_LED_5            (5u)
#define PIN_LED              PIN_LED_5
#define LED_BUILTIN          PIN_LED_5

/*
 * Analog pins
 */
#define PIN_A0               (14u)
#define PIN_A1               (15u)
#define PIN_A2               (16u)
#define PIN_A3               (17u)
#define PIN_A4               (18u)
#define PIN_DAC0             (14u)
#define PIN_DAC1             (15u)

static const uint8_t A0  = PIN_A0;
static const uint8_t A1  = PIN_A1;
static const uint8_t A2  = PIN_A2;
static const uint8_t A3  = PIN_A3;
static const uint8_t A4  = PIN_A4;
static const uint8_t DAC0 = PIN_DAC0;
static const uint8_t DAC1 = PIN_DAC1;
#define ADC_RESOLUTION		12
#define DAC_RESOLUTION		12

/*
 * Serial interfaces
 */

#define SERIAL_INTERFACES_COUNT 3

#define PIN_SERIAL1_RX       (0ul)
#define PIN_SERIAL1_TX       (1ul)

#define PIN_SERIAL2_RX       (8ul)
#define PIN_SERIAL2_TX       (9ul)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 2

#define PIN_SPI_MISO         (12u)
#define PIN_SPI_MOSI         (11u)
#define PIN_SPI_SCK          (13u)

#define PIN_SPI1_MISO        (4u)
#define PIN_SPI1_MOSI        (5u)
#define PIN_SPI1_SCK         (3u)

static const uint8_t SS	  = 10;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA         (20u)
#define PIN_WIRE_SCL         (21u)

#define PIN_WIRE1_SDA        (4u)
#define PIN_WIRE1_SCL        (3u)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

/*
 * I2S Interfaces
 */
#define I2S_INTERFACES_COUNT 1

#define PIN_I2S_SCK          (3u)
#define PIN_I2S_FS           (PIN_A0)
#define PIN_I2S_SD           (5u)
#define PIN_I2S_MCK          (4u)

/*
 * USB
 */
#define PIN_USB_VBUS        (27u)
#define PIN_USB_DM          (28u)
#define PIN_USB_DP          (29u)

#define PWM_INSTANCE_COUNT  4

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern CDC  Serial;
extern Uart Serial1;
extern Uart Serial2;
#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_USBVIRTUAL      Serial
#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE1       Serial1
#define SERIAL_PORT_HARDWARE2       Serial2
#define SERIAL_PORT_HARDWARE_OPEN1  Serial1
#define SERIAL_PORT_HARDWARE_OPEN2  Serial2

// Alias SerialUSB to Serial
#define SerialUSB SERIAL_PORT_USBVIRTUAL

#endif /* _VARIANT_QUANTUM_STM32L432KC_ */

