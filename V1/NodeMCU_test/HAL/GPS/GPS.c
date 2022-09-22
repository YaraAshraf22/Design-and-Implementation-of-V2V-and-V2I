/*
* GPS.c
*
* Created: 06-Feb-22 9:19:00 PM
*  Author: hp
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "STD_TYPES.h"
#include "MC_REG.h"
#include "BIT_MATH.h"

#include "GPS.h"
#include "GPS_CFG.h"

//#include "UART.h"
#include "SWUART.h"

#define F_CPU 16000000UL
#include "avr/delay.h"




//use extern to be seen in main.c
u8 GPRMC[]= {'$','G','P','R','M','C'};
u8 RMC_data[80]; //avg RMC length 73 char


u8*p_comma[12];			//array of pointers


u8 UTC_TIME[]={0,0,':',0,0,':',0,0,'\0'};
u8 Status;
u8 Latitude[16];
u8 Longitude[16];
u8 Speed_Knots[8];
u8 Speed_m_s[8];
u8 Date[]={0,0,'/',0,0,'/',0,0,'\0'};

#define pi	3.14159265358979323846
#define R	6371					//Earth Radius in Km
#define knots	1.944				//1m/s=1.944knots

#include "global_var.h"


void GPS_INIT (void)
{
	//UART_INIT();
	//CLR_BIT(UCSRB,3);		//disable UART Tx
	
	SWUART_INIT();
}


void GPS_EXTRACT_RMC (void)
{
	u8 i;
	for(i=0;i<80;i++)	//clear data
	{
		RMC_data[i]='\0';
	}
	while ( GPS_Connection_Failed == 0)
	{
		u8 x=0;
		u8 index=0;
		//x=UART_RECEIVE();
		x=SWUART_RX_BYTE();
		if(x=='$')
		{
			RMC_data[0]=x;
			
			for(i=1;i<6;i++)
			{
				//RMC_data[i]=UART_RECEIVE();
				RMC_data[i]=SWUART_RX_BYTE();
			}
			if(RMC_data[1]==GPRMC[1] && RMC_data[2]==GPRMC[2] && RMC_data[3]==GPRMC[3] && RMC_data[4]==GPRMC[4] && RMC_data[5]==GPRMC[5]) // if GPRMC
			{
				u8 c=0;
				for(i=6;i<80;i++)					//extract data
				{
					
					//RMC_data[i]=UART_RECEIVE();
					RMC_data[i]=SWUART_RX_BYTE();
					
					if(RMC_data[i]==',')			//save comma location
					{
						p_comma[c]=&RMC_data[i];
						c++;
						
					}
					else if(RMC_data[i]=='*')		//end of data
					{
						return;							//exit function
					}
				}
			}
		}
	}
}






void GPS_GET_TIME (void)
{
	
	u8 i,k;
	u8 index =0;
	
	for(i=0;i<80;i++)
	{
		if (&RMC_data[i]==p_comma[0])	//at address of 1nd Comma
		{
			index=i+1;
			break;
		}
	}
	
	for(i=index,k=0; i<(index+6),k<8 ; i++,k++)
	{
		if (UTC_TIME[k]==':')
		{
			UTC_TIME[++k]=RMC_data[i];
		}
		else
		{
			UTC_TIME[k]=RMC_data[i];
		}
	}
	
	
}


void GPS_GET_STATUS (void)
{
	
	
	u8 i;
	u8 index=0;
	
	for(i=0;i<80;i++)
	{
		if (&RMC_data[i]==p_comma[1])	//at address of 2nd Comma
		{
			index=i+1;
			break;
		}
	}
	
	Status=RMC_data[index];
	
}


void GPS_GET_LATITUDE (void)
{
	u8 i,k;
	u8 index=0;
	
	
	for(i=0;i<80;i++)
	{
		if (&RMC_data[i]==p_comma[2])	//at address of 3rd Comma
		{
			index=i+1;
			break;
		}
	}
	
	
	for(i=0;i<2;i++)
	{
		Latitude[i]=RMC_data[index];
		index++;
		
	}
	
	//Latitude[2]=',';
	
	for(i=2; ;i++)
	{
		if (&RMC_data[index]==p_comma[4])
		{
			Latitude[i]='\0';
			break;
		}
		else
		{
			Latitude[i]=RMC_data[index];
			index++;
		}
		
	}
	
	
}

void GPS_GET_LONGITUDE (void)
{
	u8 i,k;
	u8 index=0;
	
	
	for(i=0;i<80;i++)
	{
		if (&RMC_data[i]==p_comma[4])
		{
			index=i+1;
			break;
		}
	}
	
	
	for(i=0;i<3;i++)
	{
		Longitude[i]=RMC_data[index];
		index++;
		
	}
	//Longitude[3]=',';
	for(i=3; ;i++)
	{
		if (&RMC_data[index]==p_comma[6])
		{
			Longitude[i]='\0';
			break;
		}
		else
		{
			Longitude[i]=RMC_data[index];
			index++;
		}
		
	}
}

void GPS_GET_DATE (void)
{
	u8 i,k;
	u8 index=0;
	
	
	for(i=0;i<80;i++)
	{
		if (&RMC_data[i]==p_comma[8])
		{
			index=i+1;
			break;
		}
	}
	
	for(i=index,k=0; i<(index+6),k<8 ; i++,k++)
	{
		if (Date[k]=='/')
		{
			Date[++k]=RMC_data[i];
		}
		else
		{
			Date[k]=RMC_data[i];
		}
	}
}




void GPS_GET_SPEED (void)
{
	u8 i,k;
	u8 index=0;

	
	for(i=0;i<80;i++)
	{
		if (&RMC_data[i]==p_comma[6])
		{
			index=i+1;
			break;
		}
	}
	for(i=index,k=0; ;i++,k++)
	{
		if (&RMC_data[i]==p_comma[7])
		{
			Speed_Knots[k]='\0';
			break;
		}
		else
		{
			Speed_Knots[k]=RMC_data[i];
			
		}
		
	}
	
	GPS_SPEED_knots_to_m_per_s();
	
}


void GPS_SPEED_knots_to_m_per_s (void)
{
	
	f32 speed_float = atof(Speed_Knots);
	speed_float=speed_float/knots;	//(knots to m/s)
	dtostrf(speed_float, 5 , 4, Speed_m_s);
	
}


f32 deg2rad (f32 deg)
{
	return (deg*pi/180);
}

f32 rad2deg (f32 rad)
{
	return (rad*180/pi);
}

f32 GPS_DEG_MIN_TO_DECIMAL_DEG(u8*coordinate)
{
	u8*ar1=coordinate;
	u8 i,Length;
	for(i=0;i<16;i++)
	{
		if (ar1[i]==',')
		{
			Length=i;
			break;
		}
	}
	ar1[i]='\0';
	
	f32 float_value = atof(ar1);
	
	/* Degree Minutes to Decimal Deg*/
	f32 Deg_min = float_value/100;
	u8 Degrees = (u8)Deg_min;
	f32 Minutes = (Deg_min-(f32)Degrees)*100;
	f32 decimal_degrees= (f32)Degrees + Minutes/60;
	
	//dtostrf(float_value, Length , 3, ar1);
	
	
	return decimal_degrees;
	
	/*dtostrf(decimal_degrees, 7 , 5, ar1);
	return ar1;*/
	

}

u8* GPS_DEG_MIN_TO_DECIMAL_DEG_STR(u8*coordinate)
{
	u8*ar1=coordinate;
	u8 i,Length;
	for(i=0;i<16;i++)
	{
		if (ar1[i]==',')
		{
			Length=i;
			break;
		}
	}
	ar1[i]='\0';
	
	f32 float_value = atof(ar1);
	
	/* Degree Minutes to Decimal Deg*/
	f32 Deg_min = float_value/100;
	u8 Degrees = (u8)Deg_min;
	f32 Minutes = (Deg_min-(f32)Degrees)*100;
	f32 decimal_degrees= (f32)Degrees + Minutes/60;
	
	
	dtostrf(decimal_degrees, 7 , 5, ar1);
	return ar1;
	

}


u8* GPS_CALC_DISTANCE	(u8*lat_1,u8*long_1,u8*lat_2,u8*long_2, u8 unit)
{
	
	f32 distance=0;
	
	//convert to Decimal Degree then to radians
	f32 lat1_DecDeg=	deg2rad(GPS_DEG_MIN_TO_DECIMAL_DEG(lat_1));
	f32 lat2_DecDeg=	deg2rad(GPS_DEG_MIN_TO_DECIMAL_DEG(lat_2));
	
	f32 long1_DecDeg=	deg2rad(GPS_DEG_MIN_TO_DECIMAL_DEG(long_1));
	f32 long2_DecDeg=	deg2rad(GPS_DEG_MIN_TO_DECIMAL_DEG(long_2));
	

	
	if(lat1_DecDeg==lat2_DecDeg && long1_DecDeg==long2_DecDeg)
	{
		distance=0;
		u8*string_distance=lat_1;
		dtostrf(distance, 1 , 0, string_distance);
		return string_distance;
	}
	
	else
	{
		f32 dLat = (lat2_DecDeg-lat1_DecDeg);
		f32 dLon = (long2_DecDeg-long1_DecDeg);
		
		f32 a = pow(sin(dLat/2),2)+ cos(lat1_DecDeg)*cos(lat2_DecDeg)*pow(sin(dLon/2),2);
		f32 c =2* atan2(sqrt(a),sqrt(1-a));
		
		
		
		switch (unit)
		{
			case km:
			{
				distance=(R*c);
				break;
			}
			case m:
			{
				distance=(R*c*1000);
				break;
			}
			case cm:
			{
				distance=(R*c*1000*100);
				break;
			}
		}
		u8*string_distance=lat_1;
		dtostrf(distance, 6 , 2, string_distance);
		return string_distance;
	}
}

