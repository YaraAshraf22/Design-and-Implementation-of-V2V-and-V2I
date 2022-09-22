/*
 * ULTRASONIC.h
 *
 * Created: 12/20/2021 6:28:24 PM
 *  Author: pc
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "TIMER.h"


#define   TRIGER_DDR    DDRD
#define   ECHO_DDR      DDRD
#define   TRIGER_PORT   PORTD
#define   ECHO_PULLUP   PORTD
#define   TRIGER        4
#define   ECHO          3

void ultrasonic_init(void);
void ultrasonic_triger(void);


u16 distance;

#endif /* ULTRASONIC_H_ */