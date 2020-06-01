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

#ifndef _VARIANT_Q40_STM32L433CC_
#define _VARIANT_Q40_STM32L433CC_

// The definitions here needs a STM32L4 core >=1.6.6
#define ARDUINO_STM32L4_VARIANT_COMPLIANCE 10606

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

#define STM32L4_CONFIG_LSECLK             32768
#define STM32L4_CONFIG_HSECLK             0
#define STM32L4_CONFIG_SYSOPT             0
#define STM32L4_CONFIG_USB_VBUS           GPIO_PIN_PB2

#define USBCON

/** Master clock frequency */
#define VARIANT_MCK			  F_CPU

// Default analog reference
#define DEFAULT  AR_DEFAULT

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
                          _____________
                         |             |
    BININP1 (D5)  (PB15) |             | GND
    BININP2 (D10) (PA8)  | O    RS485  | RS485_A (UART 1)
    OPTO COMMON          | P           | RS485_B (UART 1)
    BININP3 (D8)  (PA15) | T           |
    BININP4 (D24) (PB3)  | O           | GND
    OPTO COMMON          |      4-20mA | (PA5) (D15) ADCINP1 A1
    BININP5 (D22) (PB4)  | I           | (PA4) (D14) ADCINP2 A0
    BININP6 (D23) (PB5)  | N           |
    OPTO COMMON          | P           | (PB14) (D4) BINOUT1+
    BININP7 (D6)  (PB8)  | U    BINARY | (PB14) (D4) BINOUT1-
    BININP8 (D7)  (PB9)  | T    OUTPUT | (PA3) (D17) BINOUT2+
                         |             | (PA3) (D17) BINOUT2-
    12/24VDC             |             |
    GND                  |_____________|

                             UART2 (J1)
                          GND | | (PA9)  D30 TXD2
                          3V3 |_| (PA10) D31 RXD2

                              I2C (J14)
              SDA (D20) (PB7) | | 3V3
              SCL (D21) (PB6) |_| GND

*/

// Number of pins defined in PinDescription array
#define PINS_COUNT           (25u)
#define NUM_DIGITAL_PINS     (20u)
#define NUM_TOTAL_PINS       (41u)
#define NUM_ANALOG_INPUTS    (6u)
#define NUM_ANALOG_OUTPUTS   (2u)
#define analogInputToDigitalPin(p)  ((p < 6u) ? (p) + 14u : -1)

// SX1276 pins
#define RF_DIO0              (19u)
#define RF_DIO1              (18u)
#define RF_CS                (9u)
#define RF_RST               (2u)

// NTC pin
#define NTC_PIN              A2

// LEDs
#define PIN_LED_39           (39u)
#define PIN_LED              PIN_LED_39
#define LED_BUILTIN          PIN_LED_39

/*
 * Analog pins
 */
#define PIN_A0               (14u)
#define PIN_A1               (15u)
#define PIN_A2               (16u)
#define PIN_A3               (17u)
#define PIN_A4               (18u)
#define PIN_A5               (19u)
#define PIN_DAC0             (14u)
#define PIN_DAC1             (15u)

static const uint8_t A0  = PIN_A0;
static const uint8_t A1  = PIN_A1;
static const uint8_t A2  = PIN_A2;
static const uint8_t A3  = PIN_A3;
static const uint8_t A4  = PIN_A4;
static const uint8_t A5  = PIN_A5;
static const uint8_t DAC0 = PIN_DAC0;
static const uint8_t DAC1 = PIN_DAC1;
#define ADC_RESOLUTION		12
#define DAC_RESOLUTION		12

// Other pins

#define BOOT0_INPUT           (40u)
static const uint8_t BOOT0 = BOOT0_INPUT;

#define PIN_ATN              (38u)
static const uint8_t ATN = PIN_ATN;

/**
 * Optoisolated binary inputs
 */
#define BININP1  5
#define BININP2  10
#define BININP3  8
#define BININP4  24
#define BININP5  22
#define BININP6  23
#define BININP7  6
#define BININP8  7

#define Q40_NUMBER_BINARY_INPUTS  8

/**
 * 4-20mA analog inputs
 */
#define ADCINP1  A1
#define ADCINP2  A0

#define Q40_NUMBER_CURRENT_INPUTS  2

/**
 * Binary outputs
 */
#define BINOUT1  4
#define BINOUT2  17

#define Q40_NUMBER_BINARY_OUTPUTS  2

/*
 * Serial interfaces
 */

#define SERIAL_INTERFACES_COUNT 4

#define PIN_SERIAL1_RX       (0ul)
#define PIN_SERIAL1_TX       (1ul)

#define PIN_SERIAL2_RX       (31ul)
#define PIN_SERIAL2_TX       (30ul)

#define PIN_SERIAL3_RX       (8ul)
#define PIN_SERIAL3_TX       (9ul)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 3

#define PIN_SPI_MISO         (12u)
#define PIN_SPI_MOSI         (11u)
#define PIN_SPI_SCK          (13u)

#define PIN_SPI1_MISO        (22u)
#define PIN_SPI1_MOSI        (23u)
#define PIN_SPI1_SCK         (24u)

#define PIN_SPI2_MISO        (4u)
#define PIN_SPI2_MOSI        (5u)
#define PIN_SPI2_SCK         (3u)

static const uint8_t SS	  = 10;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 2

#define PIN_WIRE_SDA         (20u)
#define PIN_WIRE_SCL         (21u)

#define PIN_WIRE1_SDA        (4u)
#define PIN_WIRE1_SCL        (3u)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

/*
 * I2S Interfaces
 */
#define I2S_INTERFACES_COUNT 2

#define PIN_I2S_SCK          (2u)
#define PIN_I2S_FS           (3u)
#define PIN_I2S_SD           (4u)
#define PIN_I2S_MCK          (5u)

#define PIN_I2S1_SCK         (24u)
#define PIN_I2S1_FS          (21u)
#define PIN_I2S1_SD          (23u)
#define PIN_I2S1_MCK         (22u)

/*
 * USB
 */
#define PIN_USB_VBUS         (27u)
#define PIN_USB_DM           (28u)
#define PIN_USB_DP           (29u)

#define PWM_INSTANCE_COUNT   4

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
extern Uart Serial3;
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
#define SERIAL_PORT_HARDWARE3       Serial3
#define SERIAL_PORT_HARDWARE_OPEN1  Serial1
#define SERIAL_PORT_HARDWARE_OPEN2  Serial2
#define SERIAL_PORT_HARDWARE_OPEN3  Serial3

// Alias SerialUSB to Serial
#define SerialUSB SERIAL_PORT_USBVIRTUAL

#endif /* _VARIANT_Q40_STM32L433CC_ */

