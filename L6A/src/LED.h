/*
 * ECE 153B - Spring 2023
 *
 * Name(s):
 * Section:
 * Lab: 6A
 */

#ifndef __STM32L476G_DISCOVERY_LED_H
#define __STM32L476G_DISCOVERY_LED_H

#include "stm32l476xx.h"

void LED_Init(void);

void LED_Off(void);
void LED_On(void);
void LED_Toggle(void);

#endif /* __STM32L476G_DISCOVERY_LED_H */
