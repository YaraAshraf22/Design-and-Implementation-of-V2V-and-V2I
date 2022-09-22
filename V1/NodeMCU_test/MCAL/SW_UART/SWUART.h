/*
 * SWUART.h
 *
 * Created: 2/17/2022 12:49:37 PM
 *  Author: HP
 */ 


#ifndef SWUART_H_
#define SWUART_H_

#include "STD_TYPES.h"

#ifndef F_CPU
#define F_CPU	16000000UL
#endif

#define SWUART_BAUD_RATE		9600
#define SWUART_BIT_TIME			(u16)(F_CPU/SWUART_BAUD_RATE + 0.5)			//	(CLKs/BIT)
	
void SWUART_INIT (void);
u8 SWUART_RX_BYTE (void);

#endif /* SWUART_H_ */ 