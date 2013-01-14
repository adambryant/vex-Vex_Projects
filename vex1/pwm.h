/*******************************************************************************
*
*	TITLE:		pwm.h 
*
*	VERSION:	0.2 (Beta)                           
*
*	DATE:		15-May-2010
*
*	AUTHOR:		Kevin Watson
*				vex@kevin.org
*
*	COMMENTS:
*
*	LEGALESE:	This software is licensed under the simplified BSD license,
*				a copy of which is included with the original distribution.
*
*				Copyright ©2006-2010 R. Kevin Watson. All rights are reserved.
*
********************************************************************************
*
*	CHANGE LOG:
*
*	DATE         REV  DESCRIPTION
*	-----------  ---  ----------------------------------------------------------
*	29-Dec-2006  0.1  RKW - Original code.
*	15-May-2010  0.2  RKW - Added PWM2() function for very fine control
*	15-May-2010  0.2  RKW - Verified code can be built with Microchip C18 3.35
*
*******************************************************************************/
#ifndef _PWM_H
#define _PWM_H

// These values define how much each respective PWM output 
// pulse will increase or decrease relative to the center/
// neutral pulse width (defined below) for each PWM step.
// The default value of fifty provides for a pulse width
// range of 1.28 ms (256*0.000005=0.00128), which is the
// same provided by Generate_Pwms(). If you're using
// servos, you should consider decreasing the gain to
// 40 or less. 
#define PWM_1_GAIN   39	// 5.0 microseconds per step
#define PWM_2_GAIN   39	// 5.0 microseconds per step
#define PWM_3_GAIN   39	// 5.0 microseconds per step
#define PWM_4_GAIN   39	// 5.0 microseconds per step

// These values define how wide each respective center/
// neutral pulse is with a PWM input value of 127. The 
// default value of 15000 provides for a pulse width of 
// 1.5 ms, which is the same provided by Generate_Pwms().
#define PWM_1_CENTER 15000 // 1.5 milliseconds
#define PWM_2_CENTER 15000 // 1.5 milliseconds
#define PWM_3_CENTER 15000 // 1.5 milliseconds
#define PWM_4_CENTER 15000 // 1.5 milliseconds

#define HIBYTE(value) ((unsigned char)(((unsigned int)(value)>>8)&0xFF))
#define LOBYTE(value) ((unsigned char)(value))

// function prototypes
void Initialize_PWM(void);
void PWM(unsigned char,unsigned char,unsigned char,unsigned char);
void PWM2(unsigned int, unsigned int, unsigned int, unsigned int);

#endif
