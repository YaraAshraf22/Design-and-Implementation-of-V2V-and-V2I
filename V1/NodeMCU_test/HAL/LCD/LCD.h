/*
 * LCD.h
 *
 * Created: 06-Nov-21 5:39:38 PM
 *  Author: hp
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "STD_TYPES.h"

#define _4_BIT_MODE         0
#define _8_BIT_MODE         1

void LCD_INIT      (void);
void LCD_WRITE_CMD (u8);
void LCD_WRITE_CHR (u8);
void LCD_WRITE_STR (u8*);
void LCD_WRITE_INT (u32);
void LCD_FLOAT_TO_STRING(f32);
void LCD_GO_TO     (u8,u8);
void LCD_CLR       (void);
void LCD_MOVING_STR (u8*);
void LCD_ARRAY_TO_STRING (u8);

#endif /* LCD_H_ */