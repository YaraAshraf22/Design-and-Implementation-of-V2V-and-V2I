/*
 * TIMER_CFG.h
 *
 * Created: 01-Jan-22 2:44:22 PM
 *  Author: hp
 */ 


#ifndef TIMER_CFG_H_
#define TIMER_CFG_H_

//TIMER0
#define TIMER0_MODE		NORMAL 

#define TIMER0_SOURCE	NO_PRESCALING	

#define OC0_MODE		OC0_SET_ON_COMPARE_MATCH	

#define OC0_Pin			PB3




#endif /* TIMER_CFG_H_ */

//CTC/clear or set on comp match application
//some MC boot up time faster than others  
//master(initiates timer/ADC/LCD..) slave (init UART only so faster then sends data)
//delay slave until master ready to RX data
// timer trigger signal on slave Vcc/reset* pin 5.5