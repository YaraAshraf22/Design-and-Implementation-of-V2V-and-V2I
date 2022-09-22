/*
 * GPS.h
 *
 * Created: 06-Feb-22 9:19:56 PM
 *  Author: hp
 */ 


#ifndef GPS_H_
#define GPS_H_

#include "STD_TYPES.h"

//RMC NMEA MSG

void GPS_INIT (void);
void GPS_EXTRACT_RMC (void);
void GPS_GET_TIME (void);
void GPS_GET_STATUS (void);
void GPS_GET_LATITUDE (void);
void GPS_GET_LONGITUDE (void);
void GPS_GET_DATE (void);
void GPS_GET_SPEED (void);
void GPS_SPEED_knots_to_m_per_s (void);




#define km	0
#define m	1
#define cm	2

f32 deg2rad (f32);
f32 rad2deg (f32);
f32 GPS_DEG_MIN_TO_DECIMAL_DEG(u8*);
u8* GPS_DEG_MIN_TO_DECIMAL_DEG_STR(u8*);
u8*	GPS_CALC_DISTANCE	(u8*,u8*,u8*,u8*,u8);



#endif /* GPS_H_ */