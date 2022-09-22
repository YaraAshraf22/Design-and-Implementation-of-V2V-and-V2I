/*
 * LCD_CFG.h
 *
 * Created: 06-Nov-21 5:39:55 PM
 *  Author: hp
 */ 


#ifndef LCD_CFG_H_
#define LCD_CFG_H_

/********_SELECT_THE_LCD_MODE_********/
/*_OPTIONS_1_-> [_4_BIT_MODE] ********/
/*_OPTIONS_2_-> [_8_BIT_MODE] ********/
#define LCD_MODE         _4_BIT_MODE

/*************************************/


#define LCD_EN_PIN       PB3
#define LCD_RS_PIN       PB1
#define LCD_RW_PIN       PB2

// #define LCD_D0_PIN       PA0      /*_THIS_PART_NEEDED_ONLY_IN_8_BIT_MODE*/
// #define LCD_D1_PIN       PA1      /*_THIS_PART_NEEDED_ONLY_IN_8_BIT_MODE*/
// #define LCD_D2_PIN       PA2      /*_THIS_PART_NEEDED_ONLY_IN_8_BIT_MODE*/
// #define LCD_D3_PIN       PA3      /*_THIS_PART_NEEDED_ONLY_IN_8_BIT_MODE*/

// #define LCD_D4_PIN       PA4
// #define LCD_D5_PIN       PA5
// #define LCD_D6_PIN       PA6
// #define LCD_D7_PIN       PA7

#define LCD_D4_PIN       PA4
#define LCD_D5_PIN       PA5
#define LCD_D6_PIN       PA6
#define LCD_D7_PIN       PA7


#define LCD_DATA_PORT     A

#define LCD_WIDTH	16 

#endif /* LCD_CFG_H_ */