/*******************************************************************************
*
*	TITLE:		gyro.h 
*
*	VERSION:	0.1 (Beta)                           
*
*	DATE:		25-Feb-2004
*
*	AUTHOR:		R. Kevin Watson
*				kevinw@jpl.nasa.gov
*
*	COMMENTS:
*
********************************************************************************
*
*	CHANGE LOG:
*
*	DATE         REV  DESCRIPTION
*	-----------  ---  ----------------------------------------------------------
*	21-Nov-2004  0.1  RKW Original
*
*******************************************************************************/

#ifndef _gyro_h
#define _gyro_h

#define SAMPLE_RATE 50				// 400Hz/8
#define ANGULAR_RATE_PER_VOLT 50	// 20mV/deg/sec
#define ADC_BITS_PER_VOLT 1024/5	// 2^10/5

#define GYRO_RATE_SCALE_FACTOR 25/103 // ANGULAR_RATE_PER_VOLT / ADC_BITS_PER_VOLT 250/1024

#define GYRO_ANGLE_SCALE_FACTOR 5L/924L // GYRO_RATE_FUDGE_FACTOR / SAMPLE_RATE 5/1024

#define TRUE 1
#define FALSE 0

void Initialize_Gyro(void);
int Gyro_Rate(void);
long int Gyro_Angle(void);
void Gyro_Bias(void);
void Gyro_Angle_Reset(void);
void Disable_Gyro(void);
void Enable_Gyro(void);
void Initialize_Timer_2(void);
void Timer_2_Int_Handler(void);

#endif //_gyro_h
