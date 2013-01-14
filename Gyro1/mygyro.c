#include <stdio.h>
#include "mygyro.h"
#include "adc.h"

int gyro_rate;
long int gyro_angle = 0;
unsigned char track_gyro_bias = TRUE;
char _enabled = FALSE;

void gyroInitialize(void)
{
	// reset the gyro angle
	gyro_angle = 0;

	// reset the gyro rate
	gyro_rate = 0;

	// force a new gyro bias calculation
	track_gyro_bias = TRUE;

	_enabled = TRUE;
}
	
void gyroUpdate(void)
{
	static int gyro_bias = 498;
	static long int reading = 0;
	static int count = 0;
	static long int accum = 0;
	
	if (_enabled)
	{
		// get the latest ADC conversion
		reading = Get_ADC_Result(1);
		//printf("Reading: %ld\r\n", reading);

		if(track_gyro_bias)
		{
			accum += reading;
			count++;
			if (count >= 7)
			{
				gyro_bias = accum / 8;
				track_gyro_bias = FALSE;
				
				accum = 0;
				count = 0;
			}	
		}
		else
		{
			gyro_rate = reading - 1009; 
			gyro_angle += gyro_rate;
		} 
	}	
}
	
long int gyroAngle(void)
{
	return (gyro_angle * GYRO_ANGLE_SCALE_FACTOR);
}
	
void gyroAngleReset(void)
{
	gyro_angle = 0L;
}
	
void gyroDisable(void)
{
	_enabled = FALSE;
}
	
void gyroEnable(void)
{
	gyroInitialize();
}
	

