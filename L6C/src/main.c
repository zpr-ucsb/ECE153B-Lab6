/*
 * ECE 153B - Spring 2023
 *
 * Name(s): Zachary Richards
 * Section: Tuesday
 * Lab: 6C
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "DMA.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
static uint32_t uwExpectedCRCValue = 0x5A60861E;	
static volatile uint32_t ComputedCRC;	
static volatile uint32_t ComputationDone = 0;	

void completeCRC(uint32_t crc){
	// Store the calculated CRC value in a volatile static variable
	ComputedCRC = crc;
	
	// Mark the computation as completed
	ComputationDone = 1;
}

int main(void) {
	uint32_t time;
	
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	
	// Initialize SysTick
	SysTick_Init();
	
	// Initialize UART
  UART2_GPIO_Init();
  UART2_Init();
  USART_Init(USART2);
	
	// Initialize LED
	LED_Init();
	
	// Initialize CRC
	CRC_Init();
	
	// Initialize DMA
	DMA_Init();
	
	while(1) {
		// Toggle the LED to show that the CRC calculation is running
		LED_Toggle();
		
		// Start timer
		startTimer();
		
		// Mark the computation as not complete
		ComputationDone = 0;
		
		// Reset the CRC in the control register
		CRC->CR = CRC_CR_RESET;
		
		// Set number of data to transfer in CNDTR in DMA
		DMA1_Channel6->CNDTR = BUFFER_SIZE;
		
		// Enable DMA channel to start data transfer and CRC computation
		DMA1_Channel6->CCR |= DMA_CCR_EN;
		
		// Wait till the computation is completed
		while (!ComputationDone);
		
		// Disable DMA channel
		DMA1_Channel6->CCR &= ~DMA_CCR_EN;
		
		// Stop timer and store the time to calculate
		time = endTimer();
		
		// Check if calculated CRC matches the expected value
		if (ComputedCRC != uwExpectedCRCValue) {
			// Turn off the LED and exit
			LED_Off();
			printf("Unexpected Val: %lu \n", ComputedCRC); 
      printf("Expected Val: %lu \n\n", uwExpectedCRCValue); 
			break;
		}
		
		// Print the time to calculate
		printf("Time to calculate: %lu ms\n", time);
		
		// Delay 1 second
		delay(100);
	}
	
	while (1); 
}
