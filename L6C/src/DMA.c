/*
 * ECE 153B - Spring 2023
 *
 * Name(s): Zachary Richards
 * Section: Tuesday
 * Lab: 6C
 */
 
#include "DMA.h"
#include "CRC.h"

void DMA_Init(void) {
  // Enable the clock for DMA
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
  
  // Wait for DMA setup (20us delay, adjust as necessary)
  for (volatile uint32_t i = 0; i < 1000; ++i);
  
  // Disable DMA channel 6
  DMA1_Channel6->CCR &= ~DMA_CCR_EN;
  
  // Set Memory-to-memory mode
  DMA1_Channel6->CCR |= DMA_CCR_MEM2MEM;
  
  // Set channel priority to high
  DMA1_Channel6->CCR |= DMA_CCR_PL;
  
  // Set peripheral size to 32-bit
  DMA1_Channel6->CCR |= DMA_CCR_PSIZE_1;
  
  // Set memory size to 32-bit
  DMA1_Channel6->CCR |= DMA_CCR_MSIZE_1;
  
  // Disable peripheral increment mode
  DMA1_Channel6->CCR &= ~DMA_CCR_PINC;
  
  // Enable memory increment mode
  DMA1_Channel6->CCR |= DMA_CCR_MINC;
  
  // Disable circular mode
  DMA1_Channel6->CCR &= ~DMA_CCR_CIRC;
  
  // Set data transfer direction to Memory-to-Peripheral
  DMA1_Channel6->CCR |= DMA_CCR_DIR;
  
  // Set the data source to the data buffer provided in CRC.h
  DMA1_Channel6->CMAR = (uint32_t)DataBuffer;
  
  // Set the data destination to the data register of the CRC block
  DMA1_Channel6->CPAR = (uint32_t)&(CRC->DR);
  
  // Disable half transfer interrupt
  DMA1_Channel6->CCR &= ~DMA_CCR_HTIE;
  
  // Disable transfer error interrupt
  DMA1_Channel6->CCR &= ~DMA_CCR_TEIE;
  
  // Enable transfer complete interrupt
  DMA1_Channel6->CCR |= DMA_CCR_TCIE;
  
  // Set interrupt priority to 0 in NVIC
  NVIC_SetPriority(DMA1_Channel6_IRQn, 0);
  
  // Enable interrupt in NVIC
  NVIC_EnableIRQ(DMA1_Channel6_IRQn);
}

void DMA1_Channel6_IRQHandler(void) {
  // Clear NVIC interrupt flag
  NVIC_ClearPendingIRQ(DMA1_Channel6_IRQn);
  
  // Check Transfer Complete interrupt flag
  if ((DMA1->ISR & DMA_ISR_TCIF6) != 0) {
    // Clear Transfer Complete interrupt flag
    DMA1->IFCR = DMA_IFCR_CTCIF6;
    
    // Mark computation as completed by calling computationComplete
    completeCRC(CRC->DR);
  }
  
  // Clear global DMA interrupt flag
  DMA1->IFCR = DMA_IFCR_CGIF6;
}
