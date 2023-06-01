/*
 * ECE 153B - Spring 2023
 *
 * Name(s): Zachary Richards
 * Section: Tuesday
 * Lab: 6A
 */
 
#include "CRC.h"

#include "CRC.h"

/**
  * @brief  Computes the 32-bit CRC of a given buffer of data word(32-bit).
  * @param  pBuffer: pointer to the buffer containing the data to be computed
  * @param  BufferLength: length of the buffer to be computed					
  * @retval 32-bit CRC
  */
 
uint32_t CRC_CalcBlockCRC(const uint32_t *pBuffer, uint32_t BufferLength) {
  // Write each word into the CRC data register
  for (uint32_t index = 0; index < BufferLength; index++) {
    CRC->DR = pBuffer[index];
  }
  
  // Read the CRC value from the data register
  return CRC->DR;
}

void CRC_Init(void) {
   // Enable CRC clock 
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN; // Enable CRC clock

    // Default CRC Polynomial (CRC->POL): 0x04C1_1DB7
    CRC->POL = 0x04C11DB7;

    // Default initial CRC Value (CRC->INIT): 0xFFFF_FFFF
    CRC->INIT = 0xFFFFFFFF;
}