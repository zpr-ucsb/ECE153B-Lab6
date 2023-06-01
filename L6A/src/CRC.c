/*
 * ECE 153B - Spring 2023
 *
 * Name(s): Zachary Richards
 * Section: Tuesday
 * Lab: 6A
 */
 
#include "CRC.h"

/**
  * @brief  Algorithm implementation of the CRC
  * @param  Crc: specifies the previous Crc data
  * @param  Data: specifies the input data
  * @retval Crc: the CRC result of the input data
  */

uint32_t CrcSoftwareFunc(uint32_t Initial_Crc, uint32_t Input_Data, uint32_t POLY)
{
  uint8_t bindex = 0;

  /* Initial XOR operation with the previous Crc value */
  uint32_t Crc = Initial_Crc ^ Input_Data;

	uint32_t cap = sizeof(Input_Data)*8;
	
  /* The CRC algorithm routine */
  for (bindex = 0; bindex < cap; bindex++)
  {
    if (Crc & 0x80000000) // If the MSB of CRC is set
    {
      Crc = (Crc << 1) ^ POLY;
    }
    else
    {
      Crc <<= 1;
    }
  }
  
  return Crc;
}

