/*
 * BUZZER.c
 *
 * Created: 31-Oct-21 1:36:30 AM
 *  Author: hp
 */ 

#include "BUZZER_CFG.h"
#include "DIO.h"					// after BUZZER CONFIGS TO REP PIN NO. FIRST
#define  F_CPU 16000000UL
#include "util/delay.h"

void BUZZER_INIT      (void)
{
	PIN_MODE(BUZZER_PIN,OUTPUT);
}
void BUZZER_ON        (void)
{
	PIN_WRITE(BUZZER_PIN,HIGH);
}
void BUZZER_OFF       (void)
{
	PIN_WRITE(BUZZER_PIN,LOW);
}
void BUZZER_ONCE      (void)
{
	PIN_WRITE(BUZZER_PIN,HIGH);
	_delay_ms(30);
	PIN_WRITE(BUZZER_PIN,LOW);
}
void BUZZER_TWICE     (void)
{
	PIN_WRITE(BUZZER_PIN,HIGH);
	_delay_ms(30);
	PIN_WRITE(BUZZER_PIN,LOW);
	_delay_ms(50);
	PIN_WRITE(BUZZER_PIN,HIGH);
	_delay_ms(30);
	PIN_WRITE(BUZZER_PIN,LOW);
}
void BUZZER_ONCE_LONG (void)
{
	PIN_WRITE(BUZZER_PIN,HIGH);
	_delay_ms(500);
	PIN_WRITE(BUZZER_PIN,LOW);
}