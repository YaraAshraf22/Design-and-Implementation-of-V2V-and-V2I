/*
 * TIMER.h
 *
 * Created: 31-Dec-21 8:30:13 PM
 *  Author: hp
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "STD_TYPES.h"

//Timer0 Modes
#define NORMAL 		0
#define CTC			1
#define PWM			2
#define FPWM		3

//OUTPUT COMPARE PIN MODE (OC0)
#define OC0_OFF							0 //Normal & CTC
#define OC0_TOGGLE_ON_COMPARE_MATCH		1 //Normal & CTC //not recomm Normal uses CPU Time - OC normal INT at comp match and at OV then clear

#define OC0_CLEAR_ON_COMPARE_MATCH		2
#define OC0_SET_ON_COMPARE_MATCH		3


//Timer CLK Source
#define TIMER_OFF						0
#define NO_PRESCALING					1
#define _8								2
#define _64								3
#define _256							4
#define _1024							5
#define EXT_CLK_FALLING_EDGE			6
#define EXT_CLK_RISING_EDGE				7


//Timer0

void TIMER_0_MODE			(void);
void TIMER_0_OC0			(void);
void TIMER_0_INT_TOV		(void);
void TIMER_0_INT_CO			(void);

void TIMER_0_INIT			(void);
void TIMER_0_SET_TIME		(u32);
void TIMER_0_PWM_SET_DUTY_CYCLE	(u8);
void TIMER_0_START			(void);
void TIMER_0_STOP			(void);




//Timer1

void TIMER_1_MODE			(void);
void TIMER_1_OC1			(void);
void TIMER_1_INT_TOV		(void);
void TIMER_1_INT_CO			(void);

void TIMER_1_INIT			(void);
void TIMER_1_PWM_SET_DUTY_CYCLE	(u8);
void TIMER_1_START			(void);
void TIMER_1_STOP			(void);


//Timer2
void TIMER_2_INIT			(void);
void TIMER_2_START			(void);
void TIMER_2_STOP			(void);
void TIMER_2_INT_TOV		(void);


#endif /* TIMER_H_ */