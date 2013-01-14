// motor.h
#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "../base_lib/vex.h"

typedef struct
{
	char port;
	char reversed;
} MotorType;

void motorOutput( MotorType *m, char val );

#endif
