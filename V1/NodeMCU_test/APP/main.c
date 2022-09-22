/*
* NodeMCU_test.c
*
* Created: 4/1/2022 5:15:30 PM
* Author : HP
*/

//#include <avr/io.h>

#include <string.h>
#include <stdlib.h>

#include "STD_TYPES.h"

#include "DIO.h"
#include "UART.h"
#include "TIMER.h"
#include "EXT_INT.h"

#include "GPS.h"
#include "LCD.h"
#include "P_B.h"
#include "BUZZER.h"
#include "ULTRASONIC.h"

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define button_pressed	1

extern u8 distance_str[10];

//GPS Data
#include "global_var.h"
u8 GPS_Connection_Failed=0;
extern u8 Status;
extern u8 Latitude[16];
extern u8 Longitude[16];
u8 Google_Maps_Link[50]="http://maps.google.com/maps?q=";


//Last GPS Location in Case No GPS Signal
u8 Latitude_Old[]="30 10.108758,N";
u8 Longitude_Old[]="031 29.536584,E";
u8 Google_Maps_Link_Old[]="https://maps.app.goo.gl/C3kNJaMk8xR9w4dU7";

//For Timer 2
static u8 num_overflows=0;
static u8 seconds=0;
static u8 minutes=0;


int main (void)
{
	GPS_INIT();
	LCD_INIT();
	BUZZER_INIT();
	
	
	UART_INIT();
	TIMER_2_INIT();
	TIMER_2_INT_TOV();
	
	LCD_WRITE_STR("Vehicle 1 Start");
	_delay_ms(2000);
	LCD_CLR();
	LCD_WRITE_STR("GPS Connecting...");
	TIMER_2_START();
	BUZZER_ONCE();
	
	//Start extracting GPS Data when Data Status is 'A' = Active
	do
	{
		GPS_EXTRACT_RMC();
		GPS_GET_STATUS();
		
	} while (Status!='A' && GPS_Connection_Failed ==0);
	
	if (Status=='A')
	{
		GPS_GET_LATITUDE();
		GPS_GET_LONGITUDE();	
	}
	
	LCD_CLR();
	LCD_WRITE_STR("Vehicle 1 Start");
	ultrasonic_init();
	
	_delay_ms(1000);

	
	while(1)
	{
		LCD_GO_TO(0,0);
		LCD_WRITE_STR("Vehicle 1 Start");
		
		ultra_triger();
		_delay_ms(15);
		if (distance<=2)
		{
			UART_TRANSMIT_STR("%"); //to alert V2 of incoming data
			_delay_ms(200);
			LCD_CLR();
			BUZZER_ONCE_LONG();
			LCD_WRITE_STR("Accident!");
			
			//send data to NodeMCU
			UART_TRANSMIT_STR("Stop, Accident!");
			UART_TRANSMIT_STR("*");
			_delay_ms(50);
		
			switch(GPS_Connection_Failed)
			{
				case 0:
				{	//GPS Connected - Send Coordinates to NodeMCU
					UART_TRANSMIT_STR(Latitude);
					UART_TRANSMIT_STR("*");
					_delay_ms(5);
					UART_TRANSMIT_STR(Longitude);
					UART_TRANSMIT_STR("*");
					_delay_ms(10);
					
					//Convert Extracted Coordinates to Decimal Degrees and Concatenate to Google Maps Link
					strcat(Google_Maps_Link,GPS_DEG_MIN_TO_DECIMAL_DEG_STR(Latitude));
					strcat(Google_Maps_Link,",");
					strcat(Google_Maps_Link,GPS_DEG_MIN_TO_DECIMAL_DEG_STR(Longitude));
					
					UART_TRANSMIT_STR(Google_Maps_Link);
					UART_TRANSMIT_STR("#");
					
					break;
				}
				case 1:
				{
					// NO GPS Signal - Send Previously Saved Coordinates to NodeMCU
					UART_TRANSMIT_STR(Latitude_Old);
					UART_TRANSMIT_STR("*");
					//_delay_ms(10);
					UART_TRANSMIT_STR(Longitude_Old);
					UART_TRANSMIT_STR("*");
					_delay_ms(10);
					UART_TRANSMIT_STR(Google_Maps_Link_Old);
					UART_TRANSMIT_STR("#");
					break;
				}
			}
			
			LCD_GO_TO(1,0);
			LCD_WRITE_STR("Alert Sent");
			break;
		}
		else if (distance>3 && distance<=10)
		{
			BUZZER_ONCE();
			LCD_GO_TO(1,0);
			LCD_WRITE_STR("Obstacle Found");
			_delay_ms(100);
		}
		else
		{
			
			LCD_GO_TO(1,0);
			LCD_WRITE_STR("                ");

		}
	}
}


ISR (TIMER2_OVF_vect)	//Give GPS 1 minute to connect to satellite
{
	//every time overflow occurs f_clk=16MHz
	//clk_cycles/sec = 16M/f_prescaler= 16M/1024= 15625 (cycles/sec)
	//overflows/sec = (clk_cycles/sec)/255 = 61 OVs/sec
	
	num_overflows++;
	if (num_overflows>60)
	{
		num_overflows=0;
		seconds++;	
		if(Status!='A')
		{
		LCD_GO_TO(1,0);
		LCD_WRITE_STR("  ");
		LCD_GO_TO(1,0);
		LCD_WRITE_INT(60-seconds);
		}
	}

	if (seconds>59)
	{
		seconds=0;
		minutes++;
	}
		
	if(minutes==1) 
	{
		minutes=0;
		BUZZER_ONCE();
		if(Status!='A')
		{
			GPS_Connection_Failed = 1;
			
			LCD_CLR();
			LCD_WRITE_STR("No GPS Signal");
			LCD_GO_TO(1,0);
			LCD_WRITE_CHR('0');
			_delay_ms(2000);
			TIMER_2_STOP();
			
		}
	}
	

}

