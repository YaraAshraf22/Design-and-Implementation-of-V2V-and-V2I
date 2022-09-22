/*
 * LCD.c
 *
 * Created: 06-Nov-21 5:39:25 PM
 *  Author: hp
 */ 

#include "LCD_CFG.h"
#include "DIO.h"
#include "LCD.h"
#define  F_CPU 16000000UL
#include "util/delay.h"
#include <stdlib.h>

void LCD_INIT      (void)
{
	#if    LCD_MODE   ==    _8_BIT_MODE
	PIN_MODE(LCD_RS_PIN,OUTPUT);
	PIN_MODE(LCD_RW_PIN,OUTPUT);
	PIN_MODE(LCD_EN_PIN,OUTPUT);
	PORT_MODE(LCD_DATA_PORT,OUTPUT);
	_delay_ms(100);
	LCD_WRITE_CMD(0X38);                     /***_THIS_COMMAND_TO_ACTIVE_8_BIT_MODE_***/
	
	#elif  LCD_MODE   ==    _4_BIT_MODE
	PIN_MODE(LCD_RS_PIN,OUTPUT);
	PIN_MODE(LCD_RW_PIN,OUTPUT);
	PIN_MODE(LCD_EN_PIN,OUTPUT);
	PIN_MODE(LCD_D4_PIN,OUTPUT);
	PIN_MODE(LCD_D5_PIN,OUTPUT);
	PIN_MODE(LCD_D6_PIN,OUTPUT);
	PIN_MODE(LCD_D7_PIN,OUTPUT);
	_delay_ms(100);
	
	/***_THIS_3_COMMANDS_TO_ACTIVE_4_BIT_MODE_***/
	LCD_WRITE_CMD(0X33);
	LCD_WRITE_CMD(0X32);
	LCD_WRITE_CMD(0X28);
	#endif
	
	LCD_WRITE_CMD(0X0C);   /***_THIS_COMMAND_TO_TURN_ON_DISPLAY_AND_TURN_OFF_CURSOR_***/
	LCD_WRITE_CMD(0X01);   /***_THIS_COMMAND_TO_CLEAR_LCD_*****************************/
	LCD_WRITE_CMD(0X06);   /***_THIS_COMMAND_TO_MOVE_CURSOR_FROM_LEFT_TO_RIGHT_********/
	LCD_WRITE_CMD(0X02);   /***_THIS_COMMAND_TO_RETURN_HOME_***************************/
}
void LCD_WRITE_CMD (u8 command)
{
	PIN_WRITE(LCD_RS_PIN,LOW);             /***_TO_SELECT_THE_COMMAND_REGISTER_******************/
	PIN_WRITE(LCD_EN_PIN,LOW);             /***_TO_WRITE_0_ON_ENABLE_PIN_************************/
	#if      LCD_MODE   ==    _8_BIT_MODE
	PORT_WRITE(LCD_DATA_PORT,command);
	PIN_WRITE(LCD_EN_PIN,HIGH);            /***_NOW_LCD_WILL_RECEIVE_THE_DATA_******************/
	_delay_ms(1);                          /***_WAIT_FOR_1_MILLI_SECOND_************************/
	PIN_WRITE(LCD_EN_PIN,LOW);             /***_TO_WRITE_0_ON_ENABLE_PIN_***********************/
	#elif    LCD_MODE   ==    _4_BIT_MODE
	/***SEND_HIGH_PART***/
	PORT_WRITE(LCD_DATA_PORT,(command & 0xf0) | (LCD_DATA_PORT & 0x0f));
	PIN_WRITE(LCD_EN_PIN,HIGH);            /***_NOW_LCD_WILL_RECEIVE_THE_DATA_******************/
	_delay_ms(1);                          /***_WAIT_FOR_1_MILLI_SECOND_************************/
	PIN_WRITE(LCD_EN_PIN,LOW);             /***_TO_WRITE_0_ON_ENABLE_PIN_***********************/
	
	/***SEND_LOW_PART***/
	PORT_WRITE(LCD_DATA_PORT,(command << 4) | (LCD_DATA_PORT & 0x0f));
	PIN_WRITE(LCD_EN_PIN,HIGH);            /***_NOW_LCD_WILL_RECEIVE_THE_DATA_******************/
	_delay_ms(1);                          /***_WAIT_FOR_1_MILLI_SECOND_************************/
	PIN_WRITE(LCD_EN_PIN,LOW);             /***_TO_WRITE_0_ON_ENABLE_PIN_***********************/
	#endif
	_delay_ms(5);                          /***_DELAY_5_MILLI_SECOND_EACH_TIME_YOU_CALL_THE_FUNCTION_***/
}
void LCD_WRITE_CHR (u8 character)
{
	PIN_WRITE(LCD_RS_PIN,HIGH);             /***_TO_SELECT_THE_DATA_REGISTER_********************/
	PIN_WRITE(LCD_EN_PIN,LOW);             /***_TO_WRITE_0_ON_ENABLE_PIN_************************/
	#if      LCD_MODE   ==    _8_BIT_MODE
	PORT_WRITE(LCD_DATA_PORT,character);
	PIN_WRITE(LCD_EN_PIN,HIGH);            /***_NOW_LCD_WILL_RECEIVE_THE_DATA_******************/
	_delay_ms(1);                          /***_WAIT_FOR_1_MILLI_SECOND_************************/
	PIN_WRITE(LCD_EN_PIN,LOW);             /***_TO_WRITE_0_ON_ENABLE_PIN_***********************/
	#elif    LCD_MODE   ==    _4_BIT_MODE
	/***SEND_HIGH_PART***/
	PORT_WRITE(LCD_DATA_PORT,(character & 0xf0) | (LCD_DATA_PORT & 0x0f));
	PIN_WRITE(LCD_EN_PIN,HIGH);            /***_NOW_LCD_WILL_RECEIVE_THE_DATA_******************/
	_delay_ms(1);                          /***_WAIT_FOR_1_MILLI_SECOND_************************/
	PIN_WRITE(LCD_EN_PIN,LOW);             /***_TO_WRITE_0_ON_ENABLE_PIN_***********************/
	
	/***SEND_LOW_PART***/
	PORT_WRITE(LCD_DATA_PORT,(character << 4) | (LCD_DATA_PORT & 0x0f));
	PIN_WRITE(LCD_EN_PIN,HIGH);            /***_NOW_LCD_WILL_RECEIVE_THE_DATA_******************/
	_delay_ms(1);                          /***_WAIT_FOR_1_MILLI_SECOND_************************/
	PIN_WRITE(LCD_EN_PIN,LOW);             /***_TO_WRITE_0_ON_ENABLE_PIN_***********************/
	#endif
	_delay_ms(5);                          /***_DELAY_5_MILLI_SECOND_EACH_TIME_YOU_CALL_THE_FUNCTION_***/
}
void LCD_WRITE_STR (u8*p)
{
	u8 i = 0;
	while (p[i] != '\0')
	{
		LCD_WRITE_CHR(p[i]);
		i++;
	}
}

void LCD_WRITE_INT (u32 num)
{
	u32 rem = 0;
	u8 arr [16];
	s8 i = 0;
	while(num != 0)
	{
		rem = num % 10;
		arr[i] = rem + 48;
		i++;
		num = num / 10;
	}
	i--;
	while(i > -1)
	{
		LCD_WRITE_CHR(arr[i]);
		i--;
	} 
}
void LCD_GO_TO     (u8 row,u8 col)
{
	u8 Loc[2] = {0x80 , 0xc0};
	LCD_WRITE_CMD(Loc[row]+col);
}
void LCD_CLR       (void)
{
	LCD_WRITE_CMD(0X01);
}

void LCD_MOVING_STR     (u8*p)
{
	u8 i;
	LCD_WRITE_CMD(0x80);               // position (0,0)
	for (i = 0; i < 16; i++)
	{
		LCD_GO_TO(0,i);
		LCD_WRITE_STR(p);
		_delay_ms(300);
		LCD_CLR();          //Clear the LCD
	}
	
	//The following statement can be deleted
	//LCD_WRITE_CMD(0x1C);              //Shift right side
	
}


void LCD_FLOAT_TO_STRING(f32 number)
{
	u16 int_part = (u16)number;
	u16 f_part = (u16)((number - (f32)int_part)*100);
	
	s8 i=0;
	u8 arr[16]={0};
	
	u8 rem=0;
	
	while (f_part!=0)
	{
		rem = f_part % 10;
		arr[i]=rem + 48;
		i++;
		f_part=f_part/10;
	}
	
	arr[i]=46;
	i++;
	
	while (int_part!=0)
	{
		rem = int_part % 10;
		arr[i]=rem + 48;
		i++;
		int_part=int_part/10;
	}
	
	i--;
	while(i>-1)	
	{
		LCD_WRITE_CHR(arr[i]);
		i--;
	}
		
}

void LCD_ARRAY_TO_STRING (u8 arr)
{
	 u8 i= strlen(arr);
	 u8 str[i+1];
	 strcpy(str,arr);
	 str[i+1]='\0';
	 LCD_WRITE_STR(str);
}