// pid.c
#include <stdio.h>

#define kP	40
#define kI	13
#define kD	2500
#define MAX_ERROR 200

int pidAdjust( int sensor, int setpoint )
{
	int error, p, i, d, output;
	static int errorSum = 0;
	static int prevError;
	static int count = 0;
	
	error = setpoint - sensor;
	errorSum += error;
	
	if (errorSum > MAX_ERROR)
		errorSum = MAX_ERROR;
	else if (errorSum < -MAX_ERROR)
		errorSum = -MAX_ERROR;
	
	p = error * kP;
	i = errorSum * kI;
	d = (error - prevError) * kD;
	
	prevError = error;
	
	output = (p + i + d) / 100;
	
	if (++count > 25)
	{
		printf("Error: %d  p: %d  i: %d  d: %d  output: %d\n", error, p, i, d, output );
		count = 0;
	}	
	
	return output;
}	
