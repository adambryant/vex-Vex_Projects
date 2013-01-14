// motor.c

#include "motor.h"

void motorOutput( MotorType *m, char val )
{
	switch (m->port)
	{
		case 1:
			pwm01 = (val + 127) * m->reversed;
			break;
		case 2:
			pwm02 = (val + 127) * m->reversed;
			break;
		case 3:
			pwm03 = (val + 127) * m->reversed;
			break;
		case 4:
			pwm04 = (val + 127) * m->reversed;
			break;
	}
}


