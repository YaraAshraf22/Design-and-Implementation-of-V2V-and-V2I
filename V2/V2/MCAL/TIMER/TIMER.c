/*
 * TIMER.c
 *
 * Created: 31-Dec-21 8:30:34 PM
 *  Author: hp
 */ 
#include "TIMER.h"
#include "TIMER_CFG.h"
#include "MC_REG.h"
#include "DIO.h"
#include "BIT_MATH.h"
#define F_CPU 16000000UL 

u32 num_of_ov;
u32 initial_value;

void TIMER_0_MODE			(void)
{
	#if  TIMER0_MODE == NORMAL
	CLR_BIT(TCCR0,3);
	CLR_BIT(TCCR0,6);
	#elif TIMER0_MODE == CTC
	CLR_BIT(TCCR0,6);
	SET_BIT(TCCR0,3);
	#elif TIMER0_MODE == PWM
	SET_BIT(TCCR0,6);
	CLR_BIT(TCCR0,3);
	#elif TIMER0_MODE == FPWM
	SET_BIT(TCCR0,6);
	SET_BIT(TCCR0,3);
	#endif
}

void TIMER_0_OC0			(void)
{
	
	PIN_MODE(OC0_Pin,OUTPUT);
	
	#if OC0_MODE   ==	OC0_OFF	
	CLR_BIT(TCCR0,4);
	CLR_BIT(TCCR0,5);
	#elif OC0_MODE ==	OC0_TOGGLE_ON_COMPARE_MATCH
	SET_BIT(TCCR0,4);
	CLR_BIT(TCCR0,5);
	#elif OC0_MODE ==	OC0_CLEAR_ON_COMPARE_MATCH
	CLR_BIT(TCCR0,4);
	SET_BIT(TCCR0,5);
	#elif OC0_MODE ==	OC0_SET_ON_COMPARE_MATCH
	SET_BIT(TCCR0,4);
	SET_BIT(TCCR0,5);
	#endif
	
}

//Interrupts

void TIMER_0_INT_TOV			(void)
{
	SET_BIT(SREG,7);			// ENABLE GLOBAL INT
	SET_BIT(TIMSK,0);			// ENABLE TIMER0 TOV INT

}

void TIMER_0_INT_OC			(void)
{
	SET_BIT(SREG,7);			// ENABLE GLOBAL INT
	SET_BIT(TIMSK,1);			// ENABLE TIMER0 OC INT
	
}





//main functions

void TIMER_0_INIT			(void)
{
	
	TIMER_0_MODE ();
	TIMER_0_INT_TOV();	//Enable Timer0 OVF INT
	//TIMER_0_OC0	();
	//TIMER_0_INT_OC();	//compare match
}

void TIMER_0_SET_TIME		(u32 desired_time)
{
	u32 tick_time   = 1024 / 16;   // result in micro second
	u32 total_ticks = ( desired_time * 1000 ) / tick_time;
	num_of_ov       = total_ticks / 256;
	initial_value   = 255 - ( total_ticks % 256 );
	if( (total_ticks % 256) != 0 )
	{
		TCNT0       = initial_value;
		num_of_ov++;
	}
}


void TIMER_0_PWM_SET_DUTY_CYCLE	(u8 duty_cycle) 
{
	if(duty_cycle<= 100)
	{
		//OCR0= (256*duty_cycle/100) -1;
		OCR0= 255*duty_cycle/100;
	}
	
	//or set OCR0=
}


void TIMER_0_START		(void) //CLK source
{
	#if  TIMER0_SOURCE == NO_PRESCALING
	SET_BIT(TCCR0,0);
	CLR_BIT(TCCR0,1);
	CLR_BIT(TCCR0,2);
	#elif TIMER0_SOURCE == _8
	CLR_BIT(TCCR0,0);
	SET_BIT(TCCR0,1);
	CLR_BIT(TCCR0,2);
	#elif TIMER0_SOURCE == _64
	SET_BIT(TCCR0,0);
	SET_BIT(TCCR0,1);
	CLR_BIT(TCCR0,2);
	#elif TIMER0_SOURCE == _256
	CLR_BIT(TCCR0,0);
	CLR_BIT(TCCR0,1);
	SET_BIT(TCCR0,2);
	#elif TIMER0_SOURCE == _1024
	SET_BIT(TCCR0,0);
	CLR_BIT(TCCR0,1);
	SET_BIT(TCCR0,2);
	#elif TIMER0_SOURCE == EXT_CLK_FALLING_EDGE
	CLR_BIT(TCCR0,0);
	SET_BIT(TCCR0,1);
	SET_BIT(TCCR0,2);
	#elif TIMER0_SOURCE == EXT_CLK_RISING_EDGE	
	SET_BIT(TCCR0,0);
	SET_BIT(TCCR0,1);
	SET_BIT(TCCR0,2);
	#endif
	
}

void TIMER_0_STOP		(void)
{
	CLR_BIT(TCCR0,0);
	CLR_BIT(TCCR0,1);
	CLR_BIT(TCCR0,2);
}







//TIMER1 for FPWM

void TIMER_1_MODE			(void)
{
	//FPWM ICR1 as Top value - Mode 14 
	CLR_BIT(TCCR1A,0);
	SET_BIT(TCCR1A,1);
	SET_BIT(TCCR1B,3);
	SET_BIT(TCCR1B,4);
	
	
}
void TIMER_1_OC1			(void)
{
	
	PIN_MODE(PD5,OUTPUT);
	
	//FPWM
	
	CLR_BIT(TCCR1A,4); //disconnect OC1B
	CLR_BIT(TCCR1A,5);
	
	CLR_BIT(TCCR1A,6); //OCR1A clear on comp match - non-inverting
	SET_BIT(TCCR1A,7);
}

void TIMER_1_INT_TOV		(void)
{
	SET_BIT(SREG,7);			// ENABLE GLOBAL INT
	SET_BIT(TIMSK,2);			// ENABLE TIMER1 TOV INT
}
void TIMER_1_INT_CO			(void)
{
	SET_BIT(SREG,7);			// ENABLE GLOBAL INT
	SET_BIT(TIMSK,4);			// ENABLE TIMER1 OC1A INT
}

void TIMER_1_INIT			(void)
{
	TIMER_1_MODE();
	TIMER_1_OC1();
	//TIMER_1_INT_TOV();
	//TIMER_1_INT_CO();
	
}

//void TIMER_1_PWM_SET_DUTY_CYCLE	(u8);

void TIMER_1_START			(void)
{
	//prescaler 8
	
	CLR_BIT(TCCR1B,0);
	SET_BIT(TCCR1B,1);
	CLR_BIT(TCCR1B,2);
	
}
 
void TIMER_1_STOP			(void)
{
	CLR_BIT(TCCR1B,0);
	CLR_BIT(TCCR1B,1);
	CLR_BIT(TCCR1B,2);
}
	

void TIMER_2_INIT			(void)
{
	//Normal Mode
	CLR_BIT(TCCR2,6);
	CLR_BIT(TCCR2,3);
}

void TIMER_2_INT_TOV		(void)
{
	SET_BIT(SREG,7);			// ENABLE GLOBAL INT
	SET_BIT(TIMSK,6);			// ENABLE TIMER2 TOV INT
}
void TIMER_2_START			(void)
{
	//prescaler 1024
	SET_BIT(TCCR2,0);
	SET_BIT(TCCR2,1);
	SET_BIT(TCCR2,2);
}
void TIMER_2_STOP			(void)
{
	CLR_BIT(TCCR2,0);
	CLR_BIT(TCCR2,1);
	CLR_BIT(TCCR2,2);
}

	
	
	
	
	
	