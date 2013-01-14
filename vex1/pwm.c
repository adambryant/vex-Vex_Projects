/*******************************************************************************
*
*	TITLE:		pwm.c
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

#include "vex.h"
#include "pwm.h"

/*******************************************************************************
*
*	FUNCTION:		Initialize_PWM()
*
*	PURPOSE:		CCP and timer initialization				
*
*	CALLED FROM:	PWM()
*
*	PARAMETERS:		none
*
*	RETURNS:		nothing
*
*	COMMENTS:
*
*******************************************************************************/
void Initialize_PWM(void)
{
	// select 16-bit read/writes to timer 3
	T3CONbits.RD16 = 1;

	// CCP2 through CCP5 will use timer 3 for compare mode
	T3CONbits.T3CCP1 = 1;
	T3CONbits.T3CCP2 = 0;

	// use a 1:1 prescale for timer 3
	T3CONbits.T3CKPS1 = 0;
	T3CONbits.T3CKPS0 = 0;

	// use the internal 10MHz clock for timer 3 (each timer "tick" equals 100ns)
	T3CONbits.TMR3CS = 0;

	// disable the timer 3 overflow interrupt
	PIE2bits.TMR3IE = 0;

	// disable the CCP interrupts
	PIE2bits.CCP2IE = 0;
	PIE3bits.CCP3IE = 0;
	PIE3bits.CCP4IE = 0;
	PIE3bits.CCP5IE = 0;	
}

/*******************************************************************************
*
*	FUNCTION:		PWM()
*
*	PURPOSE:		Replacement for IFI's Generate_Pwms() function						
*
*	CALLED FROM:	vex.c/main()
*
*	PARAMETERS:		Four unsigned char PWM position/velocity values
*					for PWM outputs 1, 2, 3 and 4.
*
*	RETURNS:		nothing
*
*	COMMENTS:		If you use PWM2(), disable the call to PWM() in vex.c/main()
*
*******************************************************************************/
void PWM(unsigned char pwm_1, unsigned char pwm_2, unsigned char pwm_3, unsigned char pwm_4)
{
	int temp_pwm_1;
	int temp_pwm_2;
	int temp_pwm_3;
	int temp_pwm_4;

	// stop timer 3
	T3CONbits.TMR3ON = 0;

	// initialize timer 3 register to zero
	TMR3H = 0;
	TMR3L = 0;

	// reset CCP hardware
	CCP2CON = 0;
	CCP3CON = 0;
	CCP4CON = 0;
	CCP5CON = 0;

	// calculate the number of 100 ns timer ticks 
	// needed to match the desired PWM pulse width 
	temp_pwm_1 = (PWM_1_GAIN * ((int)pwm_1 - 127)) + PWM_1_CENTER;
	temp_pwm_2 = (PWM_2_GAIN * ((int)pwm_2 - 127)) + PWM_2_CENTER;
	temp_pwm_3 = (PWM_3_GAIN * ((int)pwm_3 - 127)) + PWM_3_CENTER;
	temp_pwm_4 = (PWM_4_GAIN * ((int)pwm_4 - 127)) + PWM_4_CENTER;

	// load the CCP compare registers
	CCPR2L = LOBYTE((unsigned int)temp_pwm_1);
	CCPR2H = HIBYTE((unsigned int)temp_pwm_1);

	CCPR3L = LOBYTE((unsigned int)temp_pwm_2);
	CCPR3H = HIBYTE((unsigned int)temp_pwm_2);

	CCPR4L = LOBYTE((unsigned int)temp_pwm_3);
	CCPR4H = HIBYTE((unsigned int)temp_pwm_3);

	CCPR5L = LOBYTE((unsigned int)temp_pwm_4);
	CCPR5H = HIBYTE((unsigned int)temp_pwm_4);

	// disable all interrupts to prevent an interrupt routine
	// from executing after the CCP hardware is initialized
	// and the moment the timer is started
	INTCONbits.GIEH = 0;

	// setup CCP hardware for compare mode (each PWM output 
	// transitions from low to high at this point) 
	CCP2CON = 0x09;
	CCP3CON = 0x09;
	CCP4CON = 0x09;
	CCP5CON = 0x09;

	// start timer 3
	T3CONbits.TMR3ON = 1;

	//enable interrupts
	INTCONbits.GIEH = 1;
}

/*******************************************************************************
*
*	FUNCTION:		PWM2()
*
*	PURPOSE:								
*
*	CALLED FROM:	User code.
*
*	PARAMETERS:		Four unsigned int PWM position/velocity values for PWM
*					outputs 1, 2, 3 and 4. Units are in multiples of 100nS
*					(e.g., use a value of 15000 for a 1.5mS neutral pulse).
*
*	RETURNS:		nothing
*
*	COMMENTS:		This function will immediatly produce the desired pulse
*					and can be called asynchronously. As this function may
*					interfere with the PWM() function that is regularly
*					called in vex.c/main(), you may want to comment out or
*					remove this function call.		
*					
*
*******************************************************************************/
void PWM2(unsigned int pwm_1, unsigned int pwm_2, unsigned int pwm_3, unsigned int pwm_4)
{
	// stop timer 3
	T3CONbits.TMR3ON = 0;

	// initialize timer 3 register to zero
	TMR3H = 0;
	TMR3L = 0;

	// reset CCP hardware
	CCP2CON = 0;
	CCP3CON = 0;
	CCP4CON = 0;
	CCP5CON = 0;

	// load the CCP compare registers
	CCPR2L = LOBYTE(pwm_1);
	CCPR2H = HIBYTE(pwm_1);

	CCPR3L = LOBYTE(pwm_2);
	CCPR3H = HIBYTE(pwm_2);

	CCPR4L = LOBYTE(pwm_3);
	CCPR4H = HIBYTE(pwm_3);

	CCPR5L = LOBYTE(pwm_4);
	CCPR5H = HIBYTE(pwm_4);

	// disable all interrupts to prevent an interrupt routine
	// from executing after the CCP hardware is initialized
	// and the moment the timer is started
	INTCONbits.GIEH = 0;

	// setup CCP hardware for compare mode (each PWM output 
	// transitions from low to high at this point) 
	CCP2CON = 0x09;
	CCP3CON = 0x09;
	CCP4CON = 0x09;
	CCP5CON = 0x09;

	// start timer 3
	T3CONbits.TMR3ON = 1;

	//enable interrupts
	INTCONbits.GIEH = 1;
}
