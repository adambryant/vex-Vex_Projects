/*******************************************************************************
*
*	TITLE		gyro.c 
*
*	VERSION:	0.1 (Beta)                           
*
*	DATE:		21-Nov-2004
*
*	AUTHOR:		R. Kevin Watson
*				kevinw@jpl.nasa.gov
*
*	COMMENTS:
*
********************************************************************************
*
*	CHANGE LOG:
*
*	DATE         REV  DESCRIPTION
*	-----------  ---  ----------------------------------------------------------
*	21-Nov-2004  0.1  RKW Original
*
*******************************************************************************/

#include "gyro.h"
#include "ifi_aliases.h"
#include "ifi_default.h"

int gyro_rate;
long int gyro_angle = 0;
unsigned char restart_gyro = TRUE;
unsigned char track_gyro_bias = TRUE;
unsigned char old_track_gyro_bias = FALSE;


/*******************************************************************************
*
*	FUNCTION:		Initialize_Gyro()
*
*	PURPOSE:		Initializes the gyro code
*
*	CALLED FROM:	user_routines.c/User_Initialization()
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:		Place "#include "gyro.h" in the includes section
*					of user_routines.c then call Initialize_Gyro() in
*					user_routines.c/User_Initialization().
*
*******************************************************************************/
void Initialize_Gyro(void)
{
	OpenADC(ADC_FOSC_64 & ADC_RIGHT_JUST & ADC_2ANA,
          ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS);

	Initialize_Timer_2();
}

/*******************************************************************************
*
*	FUNCTION:		Gyro_Rate()
*
*	PURPOSE:
*
*	CALLED FROM:
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:
*
*******************************************************************************/
int Gyro_Rate(void)
{
	return(gyro_rate * GYRO_RATE_SCALE_FACTOR);
}

/*******************************************************************************
*
*	FUNCTION:		Gyro_Angle()
*
*	PURPOSE:
*
*	CALLED FROM:
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:
*
*******************************************************************************/
long int Gyro_Angle(void)
{
	return(gyro_angle * GYRO_ANGLE_SCALE_FACTOR);
}

/*******************************************************************************
*
*	FUNCTION:		Gyro_Bias()
*
*	PURPOSE:
*
*	CALLED FROM:
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:
*
*******************************************************************************/
void Gyro_Bias(void)
{
	track_gyro_bias = TRUE;
}

/*******************************************************************************
*
*	FUNCTION:		Gyro_Angle_Reset()
*
*	PURPOSE:
*
*	CALLED FROM:
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:
*
*******************************************************************************/
void Gyro_Angle_Reset(void)
{
	gyro_angle = 0L;
}

/*******************************************************************************
*
*	FUNCTION:		Disable_Gyro()
*
*	PURPOSE:		Disables the gyro.
*
*	CALLED FROM:
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:		To reclaim the time dedicated to servicing the gyro's 
*					timer2 interrupt, call this function.
*
*******************************************************************************/
void Disable_Gyro(void)
{
	// disable timer2's interrupt
	PIE1bits.TMR2IE = 0;

	// turn off timer 2
	T2CONbits.TMR2ON = 0;
}

/*******************************************************************************
*
*	FUNCTION:		Enable_Gyro()
*
*	PURPOSE:		Reenables the gyro functionality after its been disabled.
*
*	CALLED FROM:
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:
*
*******************************************************************************/
void Enable_Gyro(void)
{
	// reset the gyro angle
	gyro_angle = 0;

	// reset the gyro rate
	gyro_rate = 0;

	// force a new gyro bias calculation
	track_gyro_bias = TRUE;
	old_track_gyro_bias = FALSE;

	// reset the timer2 register
	TMR2 = 0x00;

	// make sure timer 2's interrupt flag is lowered
	PIR1bits.TMR2IF = 0;

	// enable timer2's interrupt
	PIE1bits.TMR2IE = 1;

	// turn on timer 2
	T2CONbits.TMR2ON = 1;
}

/*******************************************************************************
*
*	FUNCTION:		Initialize_Timer_2()
*
*	PURPOSE:		Initializes the timer 2 hardware.
*
*	CALLED FROM:	user_routines.c/User_Initialization()
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:		This sets up timer 2 to generate an interrupt at a
*					400 Hz rate.
*
*******************************************************************************/
void Initialize_Timer_2(void)  
{
	TMR2 = 0x00;			// 8-bit timer 2 register (this is readable and writable)
							//
	PR2	= 140;				// timer 2 period register - timer 2 increments to this 
							// value then resets to zero on the next clock and starts
							// all over again
							//
	T2CONbits.T2OUTPS0 = 0;	// T2OUTPS3 T2OUTPS2 T2OUTPS1 T2OUTPS0
	T2CONbits.T2OUTPS1 = 1;	//    0        0        0        0		1:1 postscaler
	T2CONbits.T2OUTPS2 = 0;	//    0        0        0        1		1:2 postscaler
	T2CONbits.T2OUTPS3 = 1;	//    0        0        1        0		1:3 postscaler
							//    0        0        1        1		1:4 postscaler
							//    0        1        0        0		1:5 postscaler
							//    0        1        0        1		1:6 postscaler
							//    0        1        1        0		1:7 postscaler
							//    0        1        1        1		1:8 postscaler
							//    1        0        0        0		1:9 postscaler
							//    1        0        0        1		1:10 postscaler
							//    1        0        1        0		1:11 postscaler
							//    1        0        1        1		1:12 postscaler
							//    1        1        0        0		1:13 postscaler
							//    1        1        0        1		1:14 postscaler
							//    1        1        1        0		1:15 postscaler
							//    1        1        1        1		1:16 postscaler
							//
	T2CONbits.T2CKPS0 = 0;	// T2CKPS1  T2CKPS0
	T2CONbits.T2CKPS1 = 1;	//    0        0	1:1 prescaler (clock = 10MHz/each tick=100ns)
							//    0        1	1:4 prescaler (clock = 2.5MHz/each tick=400ns)
							//    1        x	1:16 prescaler (clock = 625KHz/each tick=1.6us) (T2CKPS0 doesn't matter)
							//
	IPR1bits.TMR2IP = 0;	// 0: timer 2 interrupt is low priority (leave at 0 for IFI controllers)
							// 1: timer 2 interrupt is high priority
							//
	PIR1bits.TMR2IF = 0;	// 0: TMR2 to PR2 match hasn't happened (set to 0 before enabling the interrupt)
							// 1: TMR2 to PR2 match has happened
							//
	PIE1bits.TMR2IE = 1;	// 0: disable timer 2 interrupt on PR2 match
							// 1: enable timer 2 interrupt on PR2 match
							//    if the prescaler is enabled (i.e., greater than 1:1), this
							//    match will occur n times (where n is the postscaler value)
							//    before an interrupt will be generated
							//
	T2CONbits.TMR2ON = 1;	// 0: timer 2 is disabled
							// 1: timer 2 is enabled (running)
}

/*******************************************************************************
*
*	FUNCTION:		Timer_2_Int_Handler()
*
*	PURPOSE:		If enabled, the timer 2 interrupt handler is called when
*					the TMR2 register overflows	and rolls over to zero.
*
*	CALLED FROM:	user_routines_fast.c/InterruptHandlerLow()
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:		The timer 0 module is documented in the PIC18F8520
*					data sheet starting on page 131.
*
*******************************************************************************/
void Timer_2_Int_Handler(void)
{
	unsigned int temp;
	static unsigned int i = 0;
	static unsigned int array[8];
	static int gyro_bias = 498;
	static unsigned char old_track_gyro_bias = 0;

	// if we're to calculate a new gyro bias, start with
	// a fresh sample array.
	if(track_gyro_bias == TRUE && old_track_gyro_bias == FALSE)
	{
		i = 0;
	}

	// save a copy of the track_bias variable so that we'll
	// be able to know if it has changed the next time this
	// function is called.
	old_track_gyro_bias = track_gyro_bias;

	// get the latest ADC conversion
	array[i++] = ReadADC();

	// start another conversion
	ADCON0bits.GO = 1;

	if(i >= 8)
	{
		temp = 0;

		for(i=0; i<8; i++)
		{
			temp += array[i];
		}

		if(track_gyro_bias)
		{
			gyro_bias = temp / 8;
			track_gyro_bias = FALSE;
		}
		else
		{
			gyro_rate = (temp / 8 - gyro_bias); 
			gyro_angle += gyro_rate;
		} 

		i = 0;
	}	
}
