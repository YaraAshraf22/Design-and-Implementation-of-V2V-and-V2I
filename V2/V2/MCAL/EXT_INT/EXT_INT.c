/*
 * EXT_INT.c
 *
 * Created: 09-Nov-21 1:59:08 AM
 *  Author: hp
 */ 

#include "MC_REG.h"
#include "BIT_MATH.h"

void EXT_INT_0_INIT(void)
{
	SET_BIT(SREG,7);         //Enable external interrupt
	SET_BIT(GICR,6);         //Enable external interrupt 0
	SET_BIT(MCUCR,0);        //Interrupt on Rising edge
	SET_BIT(MCUCR,1);       
	
}

void EXT_INT_1_INIT(void)
{
	SET_BIT(SREG,7);         //Enable external interrupt
	SET_BIT(GICR,7);         //Enable external interrupt 1
	SET_BIT(MCUCR,2);        //Interrupt on any logical change
//	CLR_BIT(MCUCR,3);        
	
}