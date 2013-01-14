// controller.c

#include <stdio.h>
#include "controller.h"

#define CH1_CENTER	127
#define CH2_CENTER	127
#define CH3_CENTER	127
#define CH4_CENTER	127
#define CH5_CENTER	127
#define CH6_CENTER	127

Controller *_ct;

void registerController( Controller *cont )
{
	_ct = cont;
}

int limit254( int val )
{
	if ( val > 254 )
		return 254;
	else
		return val;
}

void readController()
{
	//static int delay = 0;
	
	// Read all axis from the controller inverting the Y axis readings
	_ct->ch1 = (char)(limit254((int)PWM_in1) - CH1_CENTER);
	_ct->ch2 = (char)(limit254((int)PWM_in2) - CH2_CENTER) * -1;
	_ct->ch3 = (char)(limit254((int)PWM_in3) - CH3_CENTER) * -1;
	_ct->ch4 = (char)(limit254((int)PWM_in4) - CH4_CENTER);
	_ct->ch5 = (char)(limit254((int)PWM_in5) - CH5_CENTER);
	_ct->ch6 = (char)(limit254((int)PWM_in6) - CH6_CENTER);
	
	/*
	if (delay++ > 50)
	{
		printf("ch1: %3d  ch2: %3d  ch3:  %3d  ch4: %3d  ch5: %3d  ch6: %3d\n", _ct->ch1, _ct->ch2, _ct->ch3, _ct->ch4, _ct->ch5, _ct->ch6);
		delay = 0;
	}
	*/	
}


