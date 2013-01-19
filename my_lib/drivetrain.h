// drivetrain.h

#ifndef __DRIVETRAIN_H_
#define __DRIVETRAIN_H_

#include "C:\Dev\Vex_Projects\my_lib\motor.h"

void registerMotors( MotorType *lf, MotorType *lr, MotorType *rf, MotorType *rr );
char limit(int val);
void arcadeDrive( char yAxis, char xAxis );
void tankDrive( char leftStick, char rightStick );
void holoDrive( char fwd, char slide, char rot );

#endif

