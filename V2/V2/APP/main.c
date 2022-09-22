/*
* V2.c
*
* Created: 4/19/2022 5:18:24 AM
* Author : HP
*/

//#include <avr/io.h>

#include "STD_TYPES.h"

#include "DIO.h"
#include "BIT_MATH.h"
#include "MC_REG.h"
#include "UART.h"

#include "LED.h"
#include "LCD.h"
#include "P_B.h"
#include "TEMP_S.h"
#include "BUZZER.h"
#include "SERVO.h"
#include "ULTRASONIC.h"

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define pressed	1

u8 MCU_Data[16]="";
void nodeMCU_Rx_data (void);

u8 Latitude[16]="";
u8 Longitude[16]="";
volatile u16 Temperature=0; //volatile because sensor value is continuously updated 

static u8 num_overflows=0;
static u8 seconds=0;
static u8 minutes=0;

u8 Alert_Received=0; 

void Write_nodeMCU_Rx_data (void)
{
	//MCU_Data[16]="";
	
	
	u8 i=0,j=0;
	u8 byte=0;
	
	for (j=0;j<=16;j++)
	{
		MCU_Data[j]='\0';
	}
	
	while(1)
	{
		byte=UART_RECEIVE();
		if(byte=='/')
		{
			MCU_Data[i]='\0';
			break;
		}
		else
		{
			MCU_Data[i]=byte;
			i++;
		}
		
	}

	
}




int main (void)
{
	u8 i;
	Alert_Received=0;
	
	UART_INIT();
		
	
	//LED_INIT(M_LED);
	LCD_INIT();
	BUZZER_INIT();
	TEMP_S_INIT();
	

	LCD_GO_TO(0,0);
	LCD_WRITE_STR("Vehicle 2");
	

	//Wait for V2 to Connect to V1
	Write_nodeMCU_Rx_data ();
	LCD_GO_TO(1,0);
	LCD_WRITE_STR(MCU_Data);
	_delay_ms(50);
	
	SET_BIT(UCSRB,7);	//UART RX INTERRUPT ENABLE
	SET_BIT(SREG,7);	//GLOBAL INTERRUPT ENABLE
		
	ultrasonic_init();
	_delay_ms(1000);

	
	while(1)
	{	
		
		// Measure Distance
		ultra_triger();
		_delay_ms(15);
		
		if (distance<=1 )
		{
			//SERVO_INIT();
			//SERVO_90_DEGREE();	
			
			//BUZZER_TWICE();
			
			//LCD_GO_TO(1,0);
			//LCD_WRITE_STR("Stop Vehicle  ");
			_delay_ms(50);
			//SERVO_STOP(); 
			
		}
		 else if (distance>3 && distance<=5)
		{
			SERVO_INIT();
			SERVO_n90_DEGREE();	
					
			BUZZER_ONCE();		
				
			LCD_GO_TO(1,0);
			LCD_WRITE_STR("Obstacle Found  ");
			_delay_ms(100);
			SERVO_STOP();
			
		} 
		else
		{
			// Display Engine Temperature
			LCD_GO_TO(1,0);
			LCD_WRITE_STR("Engine Temp: ");
			Temperature=TEMP_S_READ();
			LCD_GO_TO(1,13);
			LCD_WRITE_INT(Temperature);
			_delay_ms(100);
		}
		
		if (Alert_Received==1)
		{
			
			
			Write_nodeMCU_Rx_data ();
			LCD_CLR();
			LCD_GO_TO(0,0);
			LCD_WRITE_STR(MCU_Data);
			_delay_ms(50);

			Write_nodeMCU_Rx_data ();
			for(i=0;i<16;i++)
			{
				Latitude[i]=MCU_Data[i];
			}


			Write_nodeMCU_Rx_data ();
			for(i=0;i<16;i++)
			{
				Longitude[i]=MCU_Data[i];
			}
			
				
			LCD_CLR();
			LCD_GO_TO(0,0);
			LCD_WRITE_STR("Accident at:");
			_delay_ms(1000);

			LCD_CLR();
			LCD_GO_TO(0,0);
			LCD_WRITE_STR("Latitude:");
			LCD_GO_TO(1,0);
			LCD_WRITE_STR(Latitude);
			_delay_ms(2000);

			LCD_CLR();
			LCD_GO_TO(0,0);
			LCD_WRITE_STR("Longitude:");
			LCD_GO_TO(1,0);
			LCD_WRITE_STR(Longitude);
			_delay_ms(2000);
			
			//Set to check for another alert
			Alert_Received=0;
			SET_BIT(UCSRB,7);	//UART RX INTERRUPT ENABLE
			LCD_GO_TO(0,0);
			
			LCD_WRITE_STR("Vehicle 2        ");
			
		}
	}
}




ISR (USART_RXC_vect)
{
	u8 Rx_char;
	Rx_char = UDR; 		//Get Byte
	if (Rx_char=='%')
	{
			Alert_Received=1;
			CLR_BIT(UCSRB,7);	//UART RX INTERRUPT DISABLE
	}
	
}


