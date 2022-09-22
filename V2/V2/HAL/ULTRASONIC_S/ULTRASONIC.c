/*
 * ULTRASONIC.c
 *
 * Created: 12/20/2021 6:28:14 PM
 *  Author: pc
 */ 
#include "ultrasonic.h"
#include "STD_TYPES.h"
#include "EXT_INT.h"
#include "TIMER.h"
#include "LCD.h"

u8 sensor_working=0;
u8 falling_edge=0;
u16 timer_counter=0;

u8 distance_str[10];

void ultrasonic_init(void)
{
	TRIGER_DDR|=(1<<TRIGER); 
	ECHO_DDR&=~(1<<ECHO); 
	ECHO_PULLUP|=(1<<ECHO);
	EXT_INT_1_INIT();
	TIMER_0_INIT();
	TIMER_0_STOP();
	TIMER_0_SET_TIME(0);
	TIMER_0_START();
	return;
}

void ultra_triger(void)
{
	if(!sensor_working)
	{
		TRIGER_PORT|=(1<<TRIGER);
		_delay_us(15);
		TRIGER_PORT&=~(1<<TRIGER);
		sensor_working=1;
	}
}

ISR(INT1_vect){
	if(sensor_working==1)
	{
		if(falling_edge==0)
		{
			TIMER_0_SET_TIME(0);
			falling_edge=1;
			timer_counter=0;
		}
		else
		{
			distance= (timer_counter*256+TCNT0)/932.94;
			itoa(distance,distance_str,10);
			strcat(distance_str, " cm ");
			
			//LCD_GO_TO(1,0);
			//LCD_WRITE_STR(distance_str);
			_delay_ms(40);
			timer_counter=0;
			falling_edge=0;
		}
	}
}

ISR(TIMER0_OVF_vect){
	timer_counter++;
	if(timer_counter >730){
		TIMER_0_SET_TIME(0);
		sensor_working=0;
		falling_edge=0;
		timer_counter=0;
	}
	
}