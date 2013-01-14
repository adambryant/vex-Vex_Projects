// drivetrain.c

#include <stdio.h>
#include "drivetrain.h"
#include "general.h"

MotorType *_lfm;
MotorType *_lrm;
MotorType *_rfm;
MotorType *_rrm;

void registerMotors( MotorType *lf, MotorType *lr, MotorType *rf, MotorType *rr )
{
	_lfm = lf;
	_lrm = lr;
	_rfm = rf;
	_rrm = rr;
}

char limit(int val)
{
	if (val > 127)
		return(127);
	else if (val < -127)
		return(-127);
	else
		return(val);
}

void arcadeDrive( char yAxis, char xAxis )
{
	int ro, lo;
	
	ro = limit((int)yAxis - (int)xAxis);
	lo = limit((int)yAxis + (int)xAxis);
	
	motorOutput(_lfm, (char)lo);
	motorOutput(_lrm, (char)lo);
	motorOutput(_rfm, (char)ro);
	motorOutput(_rrm, (char)ro);
	
}

void tankDrive( char leftStick, char rightStick )
{
	motorOutput(_lfm, leftStick);
	motorOutput(_lrm, leftStick);
	motorOutput(_rfm, rightStick);
	motorOutput(_rrm, rightStick);
}

// Mecanum drive currently
void holoDrive( char fwd, char slide, char rot )
{ 
	char lfo, lro, rfo, rro;
	int leftF, leftR, rightF, rightR, max;
	static int delay = 0;
	
	// Do the holonomic drive voodoo here
	leftF = (int)fwd + (int)slide + (int)rot;
	leftR = (int)fwd - (int)slide + (int)rot;
	rightF = (int)fwd - (int)slide - (int)rot;
	rightR = (int)fwd + (int)slide - (int)rot;

	// Find max value
	max = ABS(leftF);

	/*	
	if (delay++ > 50)
	{
		printf("fwd: %4d  slide: %4d  rot: %4d\r\n    ", fwd, slide, rot);
		delay = 0;
	}
	*/
	
	if (ABS(leftR) > max)
		max = ABS(leftR);

	if (ABS(rightF) > max)
		max = ABS(rightF);

	if (ABS(rightR) > max)
		max = ABS(rightR);

	// This normalizes the readings to -127 to 127
	if (max > 127)
	{
		max /= 127;
		leftF = limit(leftF) / max;
		leftR = limit(leftR) / max;
		rightF = limit(rightF) / max;
		rightR = limit(rightR) / max;
	}
	
	lfo = (char)leftF;
	lro = (char)leftR;
	rfo = (char)rightF;
	rro = (char)rightR;
	
	lfo = (lfo / 10) * 10;
	lro = (lro / 10) * 10;
	rfo = (rfo / 10) * 10;
	rro = (rro / 10) * 10;
	
	motorOutput( _lfm, lfo );
	motorOutput( _lrm, lro );
	motorOutput( _rfm, rfo );
	motorOutput( _rrm, rro );
		
	/*
	if (delay++ > 50)
	{
		printf("LF: %4d  LR: %4d  RF: %4d  RR: %4d\n", lfo, lro, rfo, rro);
		delay = 0;
	}
	*/
		
}


