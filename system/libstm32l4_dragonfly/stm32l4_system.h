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

#if !defined(_STM32L4_SYSTEM_H)
#define _STM32L4_SYSTEM_H

#include "armv7m.h"

#include "stm32l4xx.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define SYSTEM_PERIPH_FLASH                0
#define SYSTEM_PERIPH_DMA1                 1
#define SYSTEM_PERIPH_DMA2                 2
#define SYSTEM_PERIPH_GPIOA                3
#define SYSTEM_PERIPH_GPIOB                4
#define SYSTEM_PERIPH_GPIOC                5
#define SYSTEM_PERIPH_GPIOD                6
#define SYSTEM_PERIPH_GPIOE                7
#define SYSTEM_PERIPH_GPIOF                8
#define SYSTEM_PERIPH_GPIOG                9
#define SYSTEM_PERIPH_GPIOH                10
#define SYSTEM_PERIPH_USB                  11
#define SYSTEM_PERIPH_ADC                  12
#define SYSTEM_PERIPH_QSPI                 13
#define SYSTEM_PERIPH_USART1               14
#define SYSTEM_PERIPH_USART2               15
#define SYSTEM_PERIPH_USART3               16
#define SYSTEM_PERIPH_UART4                17
#define SYSTEM_PERIPH_UART5                18
#define SYSTEM_PERIPH_LPUART1              19
#define SYSTEM_PERIPH_I2C1                 20
#define SYSTEM_PERIPH_I2C2                 21
#define SYSTEM_PERIPH_I2C3                 22
#define SYSTEM_PERIPH_SPI1                 23
#define SYSTEM_PERIPH_SPI2                 24
#define SYSTEM_PERIPH_SPI3                 25
#define SYSTEM_PERIPH_SDIO                 26
#define SYSTEM_PERIPH_SAI1                 27
#define SYSTEM_PERIPH_SAI2                 28
#define SYSTEM_PERIPH_DFSDM                29
#define SYSTEM_PERIPH_CAN                  30
#define SYSTEM_PERIPH_TIM1                 31
#define SYSTEM_PERIPH_TIM2                 32
#define SYSTEM_PERIPH_TIM3                 33
#define SYSTEM_PERIPH_TIM4                 34
#define SYSTEM_PERIPH_TIM5                 35
#define SYSTEM_PERIPH_TIM6                 36
#define SYSTEM_PERIPH_TIM7                 37
#define SYSTEM_PERIPH_TIM8                 38
#define SYSTEM_PERIPH_TIM15                39
#define SYSTEM_PERIPH_TIM16                40
#define SYSTEM_PERIPH_TIM17                41
#define SYSTEM_PERIPH_LPTIM1               42
#define SYSTEM_PERIPH_LPTIM2               43
#define SYSTEM_PERIPH_DAC                  44

extern void     stm32l4_system_periph_reset(unsigned int periph);
extern void     stm32l4_system_periph_enable(unsigned int periph);
extern void     stm32l4_system_periph_disable(unsigned int periph);
extern void     stm32l4_system_periph_wake(unsigned int periph);
extern void     stm32l4_system_periph_sleep(unsigned int periph);
extern void     stm32l4_system_periph_cond_enable(unsigned int periph, volatile uint32_t *p_mask, uint32_t mask);
extern void     stm32l4_system_periph_cond_disable(unsigned int periph, volatile uint32_t *p_mask, uint32_t mask);
extern void     stm32l4_system_periph_cond_wake(unsigned int periph, volatile uint32_t *p_mask, uint32_t mask);
extern void     stm32l4_system_periph_cond_sleep(unsigned int periph, volatile uint32_t *p_mask, uint32_t mask);
extern void     stm32l4_system_bootloader(void);
extern bool     stm32l4_system_configure(uint32_t sysclk, uint32_t hclk, uint32_t pclk1, uint32_t pclk2, bool clk48);
extern bool     stm32l4_system_clk48_enable(void);
extern bool     stm32l4_system_clk48_disable(void);
extern uint32_t stm32l4_system_sysclk(void);
extern uint32_t stm32l4_system_hclk(void);
extern uint32_t stm32l4_system_pclk1(void);
extern uint32_t stm32l4_system_pclk2(void);
extern bool     stm32l4_system_suspend(void);
extern bool     stm32l4_system_resume(void);
extern bool     stm32l4_system_stop(void);
extern bool     stm32l4_system_standby(void);
extern bool     stm32l4_system_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* _STM32L4_SYSTEM_H */
