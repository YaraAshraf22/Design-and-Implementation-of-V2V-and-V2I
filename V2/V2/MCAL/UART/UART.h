/*
 * UART.h
 *
 * Created: 03-Feb-22 1:07:27 AM
 *  Author: hp
 */ 


#ifndef UART_H_
#define UART_H_

#include "STD_TYPES.h"

void UART_INIT (void);
void UART_TRANSMIT_CHR (u8);
void UART_TRANSMIT_STR (u8*);
void UART_TRANSMIT_INT  (u8);
u8	 UART_RECEIVE (void);


#endif /* UART_H_ */