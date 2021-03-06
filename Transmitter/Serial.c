/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low level serial routines
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2010 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "LPC11xx.h"                            /* LPC11xx definitions        */


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_init (void) {
   
  /* configure PINs GPIO1.6, GPIO1.7 for UART */
  LPC_SYSCON->SYSAHBCLKCTRL |= ((1UL <<  6) |   /* enable clock for GPIO      */
                                (1UL << 16) );  /* enable clock for IOCON     */

  LPC_IOCON->PIO1_6  =  (1UL <<  0);            /* P1.6 is RxD                */
  LPC_IOCON->PIO1_7  =  (1UL <<  0);            /* P1.7 is TxD                */
  LPC_IOCON->PIO1_8  =  (1UL <<  0);			/* P1.8 is Dir                */
  LPC_GPIO1->DIR    |=  (1UL <<  8);			/* Set Dir pin to ouput       */
  LPC_GPIO1->DATA   &= ~(1UL <<  8);            /* Initialize Dir to RX mode     */
  
  /* configure UART0 */
  //Baud = PCLK/(16*(DLL+DLM<<8)*(1+DIV/MUL))
  LPC_SYSCON->SYSAHBCLKCTRL |=  (1UL << 12);    /* Enable clock to UART       */
  LPC_SYSCON->UARTCLKDIV     =  (2UL <<  0);    /* UART clock =  CCLK / 2  = 24MHz   */

  LPC_UART->LCR = 0x83;                   /* 8 bits, no Parity, 1 Stop bit    */
  LPC_UART->DLL = 1;                      /* 1,000,000 Baud Rate @ 24.0 MHZ PCLK */
  LPC_UART->FDR = 0x21;                   /* FR 1.5, DIVADDVAL 1, MULVAL 2  */
  LPC_UART->DLM = 0;                      /* High divisor latch = 0           */
  LPC_UART->LCR = 0x03;                   /* DLAB = 0                         */
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int sendchar (int c) {
  LPC_GPIO1->DATA |= (1<<8);
  while (!(LPC_UART->LSR & 0x20));
  LPC_UART->THR = c;
  LPC_GPIO1->DATA &= ~(1<<8);
  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int getkey (void) {

  while (!(LPC_UART->LSR & 0x01));
  return (LPC_UART->RBR);
}
