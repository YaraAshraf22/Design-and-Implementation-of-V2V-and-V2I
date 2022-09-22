/*
 * SERVO.c
 *
 * Created: 23-Jan-22 11:18:41 PM
 *  Author: hp
 */ 


#include "SERVO.h"
#include "SERVO_CFG.h"

#include "DIO.h"
#include "TIMER.h"

#include "MC_REG.h"

#include "util/delay.h"
/*

fservo_OC=50Hz
T=20ms
foc=16M/(8)(1+Top)

mode 14: FPWM - ICR1 top value - OCRA1 for pulse


*/


void SERVO_INIT (void)
{
	TIMER_1_INIT();
	TIMER_1_START();
	ICR1=39999;
	
}

void SERVO_n90_DEGREE	(void)  //1ms
{
	OCR1A=1999;
}

void SERVO_0_DEGREE (void)		//1.5ms
{
	OCR1A=2999;
}

void SERVO_90_DEGREE (void)		//2ms
{
	OCR1A=3999;
}

void SERVO_STOP			(void)
{
	TIMER_1_STOP();
}


//without timer1

void SERVO_INIT1			(void)
{
	PIN_MODE(PC1,OUTPUT);
}
void SERVO_0_DEGREE1		(void)
{
	PIN_WRITE(PC1,HIGH);
	_delay_us(1000);
	PIN_WRITE(PC1,LOW);
}
void SERVO_90_DEGREE1		(void)
{
	PIN_WRITE(PC1,HIGH);
	_delay_us(1500);
	PIN_WRITE(PC1,LOW);
}
void SERVO_n90_DEGREE1		(void)
{
	PIN_WRITE(PC1,HIGH);
	_delay_us(2000);
	PIN_WRITE(PC1,LOW);
}
