/*
 * ECE 153B - Spring 2023
 *
 * Name(s): Zachary Richards
 * Section: Tuesday
 * Lab: 6A
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
#define uwExpectedCRCValue 0x5A60861E

static volatile uint32_t Software_ComputedCRC;

int main(void) {
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 

	// Initialize SysTick
	SysTick_Init();
	
	// Initialize LED
	LED_Init();  
	
	// Initialize UART
	UART2_Init();
	UART2_GPIO_Init();
	USART_Init(USART2);
	
	while(1) {
		LED_Toggle(); // Toggle the LED to indicate it's running
		
		startTimer(); // Start the timer
		
		uint32_t crc = 0xFFFFFFFF; // Initialize CRC with default initial value
		uint32_t poly = POLYNOME; // Polynomial for CRC calculation
		
		for (int i = 0; i < BUFFER_SIZE; i++) {
			crc = CrcSoftwareFunc(crc, DataBuffer[i], poly); // Compute CRC for each element in the buffer
		}
		
		uint32_t time = endTimer(); // Stop the timer and read the time span
		
		if (crc != uwExpectedCRCValue) {
			LED_Off(); // Turn off the LED to show the status
			printf("Unexpected Val: %lu \n", crc); 
			printf("Expected Val: %lu \n\n", uwExpectedCRCValue); 
			break; // Exit the while loop if CRC is different from expected value
		}
		
		// Print the time to calculate
		printf("Time to calculate: %lu ms\n", time);
		
		// Delay 1 second 
		delay(100);
	}
}
