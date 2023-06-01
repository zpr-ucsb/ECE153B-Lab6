/*
 * ECE 153B - Spring 2023
 *
 * Name(s): Zachary Richards
 * Section: Tuesday
 * Lab: 6B
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
static uint32_t uwExpectedCRCValue = 0x5A60861E;

int main(void) {
  static uint32_t ComputedCRC;
  uint32_t time;
  
  // Switch System Clock = 80 MHz
  System_Clock_Init(); 
	
	// Initialize SysTick
  SysTick_Init();
	
	// Initialize LED
  LED_Init();	
	
	// Initialize CRC
  CRC_Init();
	
	// Initialize UART
  UART2_GPIO_Init();
  UART2_Init();
  USART_Init(USART2);
  
	printf("ENABLE\n");
	
	// Reset CRC
	CRC_Init();
	
  while (1) {
    // Toggle LED
    LED_Toggle();

    // Start timer
    startTimer();

    // Reset CRC
    CRC_Init();

    // Compute CRC
    ComputedCRC = CRC_CalcBlockCRC(DataBuffer, BUFFER_SIZE);

    // Stop timer
    time = endTimer();

    // If CRC doesn't match expected CRC, turn LED off and break
    if (ComputedCRC != uwExpectedCRCValue) {
      LED_Off();
      printf("Unexpected Val: %lu \n", ComputedCRC); 
      printf("Expected Val: %lu \n\n", uwExpectedCRCValue); 
      break;
    }

    // Print the time to calculate
    printf("Time to calculate: %lu ms\r\n", time);

    // Delay 1 second
    delay(100);
  }
}
