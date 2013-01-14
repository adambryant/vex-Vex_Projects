/*******************************************************************************
*
*	TITLE:		user_code.c 
*
*	VERSION:	0.1 (Beta)                           
*
*	DATE:		15-May-2010
*
*	AUTHOR:		Kevin Watson
*				vex@kevin.org
*
*	COMMENTS:	This file best viewed with tabs set to four.
*
*	LEGALESE:	This software is licensed under the simplified BSD license,
*				a copy of which is included with the original distribution.
*
*				Copyright ©2008-2010 R. Kevin Watson. All rights are reserved.
*
********************************************************************************
*
*	Change log:
*
*	DATE         REV  DESCRIPTION
*	-----------  ---  ----------------------------------------------------------
*	15-May-2010  0.1  RKW Original
*
*******************************************************************************/

#include <stdio.h>
#include "../base_lib/adc.h"
#include "../base_lib/vex.h"
#include "../base_lib/serial_ports.h"
#include "../base_lib/interrupts.h"
#include "../base_lib/timers.h"
#include "../base_lib/pwm.h"
#include "../my_lib/gyro.h"
#include "../my_lib/controller.h"
#include "../my_lib/drivetrain.h"
#include "user_code.h"
#include "pid.h"

MotorType flm, rlm, frm, rrm;
Controller ct;

/*******************************************************************************
*
*	FUNCTION:		User_Initialization()
*
*	PURPOSE:		This function is called once when the robot controller
*					is cold or warm booted. You should initialize your code
*					here.		
* 
*	CALLED FROM:	main() in vex.c
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
* 
*	COMMENTS:
*
*******************************************************************************/
void User_Initialization(void)
{
	// Configure the number of analog channels. Analog channels are
	// always contiguous and start at I/O pin one.
	Set_Num_ADC_Channels(4);

	// Setup the I/O pins. Use "INPUT" to setup the pin as an input and
	// "OUTPUT" to setup the pin as an output. Make sure all analog inputs
	// and unused pins are set to "INPUT".
	IO1 = INPUT;
	IO2 = INPUT;
	IO3 = INPUT;
	IO4 = INPUT;
	IO5 = INPUT;
	IO6 = INPUT;
	IO7 = INPUT;
	IO8 = INPUT;
	IO9 = INPUT;
	IO10 = INPUT;
	IO11 = INPUT;
	IO12 = INPUT;
	IO13 = INPUT;
	IO14 = INPUT;
	IO15 = INPUT;
	IO16 = INPUT;


	// Initialize the digital outputs. If the pin is configured
	// as an input above, it doesn't matter what state you
	// initialize it to here.
	rc_dig_out01 = 0;
 	rc_dig_out02 = 0;
	rc_dig_out03 = 0;
	rc_dig_out04 = 0;
	rc_dig_out05 = 0;
	rc_dig_out06 = 0;
	rc_dig_out07 = 0;
 	rc_dig_out08 = 0;
	rc_dig_out09 = 0;
	rc_dig_out10 = 0;
	rc_dig_out11 = 0;
	rc_dig_out12 = 0;
	rc_dig_out13 = 0;
 	rc_dig_out14 = 0;
	rc_dig_out15 = 0;
	rc_dig_out16 = 0;


	// Set initial PWM values.
	pwm01 = 127;
	pwm02 = 127;
	pwm03 = 127;
	pwm04 = 127;
	pwm05 = 127;
	pwm06 = 127;
	pwm07 = 127;
	pwm08 = 127;

	// Call other initialization functions here

	// Initialize and reset the millisecond clock.
	//Initialize_Timer_2();
	//Set_Time(0);

	// Initialize the analog-to-digital converter (ADC) software.
	Initialize_ADC();

	// Initialize the PWM() software.
	Initialize_PWM();
	
	// Initialize Gyro
	gyroInitialize();
	
    // Set up motors
    flm.port = 1;
    flm.reversed = -1;
    rlm.port = 2;
    rlm.reversed = -1;
    
    frm.port = 3;
    frm.reversed = 1;
    rrm.port = 4;
    rrm.reversed = 1;
    
    registerMotors(&flm, &rlm, &frm, &rrm);
    
    // Set up Controller
    registerController(&ct);
}

/*******************************************************************************
*
*	FUNCTION:		User_Code()
*
*	PURPOSE:				
* 
*	CALLED FROM:	main() in vex.c
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
* 
*	COMMENTS:		This function gets called every 17mS.		
*
*******************************************************************************/
void User_Code(void)
{
	static unsigned int i = 0;
	unsigned long time;
	//unsigned int adcVal = 0;
	long int angle = 0;
	static int desiredHeading = 0;
	
	gyroUpdate();

	readController();
	
	if ( ct.ch5 > 20 )
		desiredHeading -= 1;
	if (ct.ch6 > 20 )
		desiredHeading += 1;
	
//	holoDrive(ct.ch2, ct.ch1, ct.ch3);
	angle = gyroAngle();
	holoDrive(ct.ch2, ct.ch1, pidAdjust(angle, desiredHeading));

	// increment the loop counter
	i++;

	// have we executed 25 loops yet?
	if(i == 25)
	{
		// get the current time...
		//time = Get_Time();
		

		// ...and send it to the console screen
		// Note: because the maximum unsigned integer value is 65535,
		// the time will roll over to zero every 65536 milliseconds,
		//printf("Elapsed Time = %u mS\r\n", (unsigned int)time);
		//printf("  Angle: %ld\r\n", angle );

		// reset the loop counter
		i = 0;
		
	}
}

/*******************************************************************************
*
*	FUNCTION:		User_Code_Spin()
*
*	PURPOSE:				
* 
*	CALLED FROM:	main() in vex.c
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
* 
*	COMMENTS:		
*
*******************************************************************************/
void User_Code_Spin(void)
{

}

