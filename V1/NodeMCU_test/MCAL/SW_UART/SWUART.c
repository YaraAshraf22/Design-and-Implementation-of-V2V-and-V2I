/*
 * CFile1.c
 *
 * Created: 2/17/2022 12:48:45 PM
 *  Author: HP
 */ 

#include "DIO.h"
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "MC_REG.h"
#include "SWUART.h"
#include <avr/interrupt.h>


volatile u8 RX_DATA;
volatile u8 RX_BUFFER;
volatile u8 RX_BIT_MASK;
volatile u8 RX_BYTE_COMPLETE;

#include "global_var.h"

#include "LCD.h"
#define F_CPU 16000000UL
#include <util/delay.h>


/*
Input Capture Interrupt to detect start bit
TCNT1 value stored to ICR1 register
*/
ISR(TIMER1_CAPT_vect)
{
	
	OCR1B= ICR1 + (u16)(SWUART_BIT_TIME*1.5);	//OC1B INT after 1.5 Bit Time
	RX_BUFFER=0;								//clear temporary buffer
	RX_BIT_MASK=1;								//1st Bit
	
	
	if (!GET_BIT(PIND,6))	//If RX pin == Low (Start Bit)
	{
		CLR_BIT(TIMSK,5);	//Disable input capture interrupt
		SET_BIT(TIFR,3);	//clear OCRF1B 
		SET_BIT(TIMSK,3);	//Enable output compare match B (to receive bits)
	}
	
}



/*
OUTPUT Compare B Interrupt to read data after start bit is detected
*/
ISR(TIMER1_COMPB_vect)
{
	
	if(RX_BIT_MASK)
	{
		if(GET_BIT(PIND,6))				//If Rx Bit==HIGH							
		{
			RX_BUFFER|=RX_BIT_MASK;		//Set Bit
		}
		RX_BIT_MASK<<=1;				
		OCR1B+= SWUART_BIT_TIME;		//Next OC INT after 1 Bit Time
	}
	else
	{
		RX_BYTE_COMPLETE=1;	//RX Byte Complete
		RX_DATA=RX_BUFFER;	//Save Data
		SET_BIT(TIFR,5);	//clear ICF1 to read new byte
		SET_BIT(TIMSK,5);	//Enable ICP1 Interrupt
		CLR_BIT(TIMSK,3);	//Disable OCRB1 Interrupt
	}
}

void SWUART_INIT(void)
{
	CLR_BIT(TCCR1A,0);	//Timer1 Mode 0 Normal (top=0xFFFF)
	CLR_BIT(TCCR1A,1); 
	CLR_BIT(TCCR1B,3);
	CLR_BIT(TCCR1B,4);
	
	SET_BIT(TCCR1B,7);	//Noise Canceler
	CLR_BIT(TCCR1B,6);	//ICP1 Falling Edge
	
	
	SET_BIT(TIFR,5);	//Clear ICP1 flag
	SET_BIT(TIMSK,5);	//ICP1 Interrupt Enable
	SET_BIT(SREG,7);	//Global Interrupt Enable
	
	TCNT1=0;			//Clear Counter
	
	SET_BIT(TCCR1B,0);	//Start Timer1 - No Presaler
	CLR_BIT(TCCR1B,1);
	CLR_BIT(TCCR1B,2);
	
	RX_BYTE_COMPLETE=0;			

}

u8 SWUART_RX_BYTE(void)
{
	while(!RX_BYTE_COMPLETE && GPS_Connection_Failed == 0)
	{

	}
	RX_BYTE_COMPLETE=0;
	return RX_DATA;
}