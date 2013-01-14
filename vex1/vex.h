/*******************************************************************************
*
*	TITLE:		vex.h 
*
*	VERSION:	0.1 (Beta)                           
*
*	DATE:		15-May-2010
*
*	AUTHOR:		Kevin Watson
*				vex@kevin.org
*
*	COMMENTS:	This file contains the configuration data for the startup and 
*				interrupt service routine dispatch code for the Vex robot
*				controller. 
*
*				This file best viewed with tabs set to four.
*
*	LEGALESE:	This software is licensed under the simplified BSD license,
*				a copy of which is included with the original distribution.
*
*				Portions are copyright ©2008-2010 R. Kevin Watson. All rights
*				are reserved.
*
********************************************************************************
*
*	Change log:
*
*	DATE         REV  DESCRIPTION
*	-----------  ---  ----------------------------------------------------------
*	01-Mar-2008  0.1  RKW Original
*	15-May-2010  0.1  RKW - Verified code can be built with Microchip C18 3.35
*
*******************************************************************************/
#ifndef _vex_h
#define _vex_h

#include <p18f8520.h>

// Remove the comment slashes from one of the two following
// lines to select the C18 version we're building with.
//   #define USE_C18_30 			// we're building with C18 3.0+
#define USE_C18_24			// we're building with C18 2.4

// Remove the comment slashes from one or more of the following lines to
// enable the respective external interrupt(s) and/or timer(s). By doing
// so, you only enable the code within vex.c to become part of your
// software build.
#define ENABLE_ADC				// enable if using ADC on a 18F8520
// #define ENABLE_INT_1			// enable if using encoder channel 1
// #define ENABLE_INT_2			// enable if using encoder channel 2
// #define ENABLE_INT_3			// enable if using encoder channel 3
// #define ENABLE_INT_4			// enable if using encoder channel 4
// #define ENABLE_INT_5			// enable if using encoder channel 5
// #define ENABLE_INT_6			// enable if using encoder channel 6
// #define ENABLE_TIMER_0		// unused
// #define ENABLE_TIMER_1		// unused
#define ENABLE_TIMER_2			// enable if using the millisecond time clock
#define ENABLE_TIMER_3	 		// leave disabled if using PWM code
#define ENABLE_TIMER_4 			// enable if using the ADC or gyro


// Remove the comment slashes to build for the MPLAB simulation tool
// #define _SIMULATOR


// DEBUG() is a macro that can be used just like printf() to send 
// debugging information to the screen. It has the added benefit that 
// it can be turned off and removed from the entire project by 
// commenting out the #define _DEBUG line below. Use it just like 
// printf(), just make sure you use double parentheses. As an example: 
// DEBUG(("Error: I'm about to cause a red-light-of-death\r\n")).
// The use of double parentheses is a common method employed to send a 
// variable number of arguments to a macro. Also, don't forget to
// include vex.h in every source file you use the DEBUG() macro.
// #define _DEBUG

//
// If you modify stuff below this line, you'll break the software.
//

// use a different tmpdata section for ISRs if we're using C18 3.0 or later
#ifdef USE_C18_30
#define USE_ALTERNATE_TMPDATA
#endif

// DEBUG macro mentioned above
#ifdef _DEBUG
#define DEBUG(x) printf x
#else
#define DEBUG(x)
#endif

// Because the MPLAB simulation tool is confused by the presence
// of the IFI boot loader, we'll just pretend we don't have a 
// boot loader and change the reset, low priority and high priority
// vectors to their normal values when we use the simulator.
#ifdef _SIMULATOR
#define RESET_VECTOR	0x000
#define HIGH_INT_VECTOR	0x008
#define LOW_INT_VECTOR	0x018
#else
#define RESET_VECTOR	0x800
#define HIGH_INT_VECTOR	0x808
#define LOW_INT_VECTOR	0x818
#endif

#define INPUT	1
#define OUTPUT	0

#define DATA_SIZE			30
#define SPI_TRAILER_SIZE	2
#define SPI_XFER_SIZE		DATA_SIZE + SPI_TRAILER_SIZE

// aliases for PWM inputs
#define PWM_in1			rxdata.oi_analog01
#define PWM_in2			rxdata.oi_analog02
#define PWM_in3			rxdata.oi_analog03
#define PWM_in4			rxdata.oi_analog04
#define PWM_in5			rxdata.oi_analog05
#define PWM_in6			rxdata.oi_analog06
#define PWM_in7			rxdata.oi_analog07
#define PWM_in8			rxdata.oi_analog08

// aliases for PWM outputs
#define pwm01			txdata.rc_pwm01
#define pwm02			txdata.rc_pwm02
#define pwm03			txdata.rc_pwm03
#define pwm04			txdata.rc_pwm04
#define pwm05			txdata.rc_pwm05
#define pwm06			txdata.rc_pwm06
#define pwm07			txdata.rc_pwm07
#define pwm08			txdata.rc_pwm08

// Used in User_Initialization routine in user_routines.c file
// Used to set pins as INPUTS (analog or digital) or OUTPUTS (digital only)
#define IO1				TRISAbits.TRISA0
#define IO2				TRISAbits.TRISA1
#define IO3				TRISAbits.TRISA2
#define IO4				TRISAbits.TRISA3
#define IO5				TRISAbits.TRISA5
#define IO6				TRISFbits.TRISF0
#define IO7				TRISFbits.TRISF1
#define IO8				TRISFbits.TRISF2
#define IO9				TRISFbits.TRISF3
#define IO10			TRISFbits.TRISF4
#define IO11			TRISFbits.TRISF5
#define IO12			TRISFbits.TRISF6
#define IO13			TRISHbits.TRISH4
#define IO14			TRISHbits.TRISH5
#define IO15			TRISHbits.TRISH6
#define IO16			TRISHbits.TRISH7

// aliases used to read the pins when used as inputs
#define rc_dig_in01		PORTAbits.RA0
#define rc_dig_in02		PORTAbits.RA1
#define rc_dig_in03		PORTAbits.RA2
#define rc_dig_in04		PORTAbits.RA3
#define rc_dig_in05		PORTAbits.RA5
#define rc_dig_in06		PORTFbits.RF0
#define rc_dig_in07		PORTFbits.RF1
#define rc_dig_in08		PORTFbits.RF2
#define rc_dig_in09		PORTFbits.RF3
#define rc_dig_in10		PORTFbits.RF4
#define rc_dig_in11		PORTFbits.RF5
#define rc_dig_in12		PORTFbits.RF6
#define rc_dig_in13		PORTHbits.RH4
#define rc_dig_in14		PORTHbits.RH5
#define rc_dig_in15		PORTHbits.RH6
#define rc_dig_in16		PORTHbits.RH7

// aliases used to drive the pins when used as outputs
#define rc_dig_out01	LATAbits.LATA0
#define rc_dig_out02	LATAbits.LATA1
#define rc_dig_out03	LATAbits.LATA2
#define rc_dig_out04	LATAbits.LATA3
#define rc_dig_out05	LATAbits.LATA5
#define rc_dig_out06	LATFbits.LATF0
#define rc_dig_out07	LATFbits.LATF1
#define rc_dig_out08	LATFbits.LATF2
#define rc_dig_out09	LATFbits.LATF3
#define rc_dig_out10	LATFbits.LATF4
#define rc_dig_out11	LATFbits.LATF5
#define rc_dig_out12	LATFbits.LATF6
#define rc_dig_out13	LATHbits.LATH4
#define rc_dig_out14	LATHbits.LATH5
#define rc_dig_out15	LATHbits.LATH6
#define rc_dig_out16	LATHbits.LATH7

// aliases for interrupts
#define rc_dig_int1		PORTBbits.RB2
#define rc_dig_int2		PORTBbits.RB3
#define rc_dig_int3		PORTBbits.RB4
#define rc_dig_int4		PORTBbits.RB5
#define rc_dig_int5		PORTBbits.RB6
#define rc_dig_int6		PORTBbits.RB7

// ADC channel identifiers for Quick_ADC() function
#define rc_ana_in01		 0
#define rc_ana_in02		 1
#define rc_ana_in03		 2
#define rc_ana_in04		 3
#define rc_ana_in05		 4
#define rc_ana_in06		 5
#define rc_ana_in07		 6
#define rc_ana_in08		 7
#define rc_ana_in09		 8
#define rc_ana_in10		 9
#define rc_ana_in11		10
#define rc_ana_in12		11
#define rc_ana_in13		12
#define rc_ana_in14		13
#define rc_ana_in15		14
#define rc_ana_in16		15


// CCP2 -> PWM OUT 1
// CCP3 -> PWM OUT 2
// CCP4 -> PWM OUT 3
// CCP5 -> PWM OUT 4

// to set the CCP pin direction (input or output) of the pins you use these aliases
#define IO_CCP2			TRISEbits.TRISE7
#define IO_CCP3			TRISGbits.TRISG0
#define IO_CCP4			TRISGbits.TRISG3
#define IO_CCP5			TRISGbits.TRISG4

// when using CCP pins as inputs you read the values with these aliases
#define IN_CCP2			PORTEbits.RE7
#define IN_CCP3			PORTGbits.RG0
#define IN_CCP4			PORTGbits.RG3
#define IN_CCP5			PORTGbits.RG4

// when using CCP pins as outputs you drive a value with these aliases
#define OUT_CCP2		LATEbits.LATE7
#define OUT_CCP3		LATGbits.LATG0
#define OUT_CCP4		LATGbits.LATG3
#define OUT_CCP5		LATGbits.LATG4

// to set the direction (output) of the PWM pins you use these aliases
#define IO_PWM05		TRISEbits.TRISE0
#define IO_PWM06		TRISEbits.TRISE1
#define IO_PWM07		TRISEbits.TRISE2
#define IO_PWM08		TRISEbits.TRISE3

// when using PWM pins as outputs you drive a value with these aliases
#define OUT_PWM01		OUT_CCP2
#define OUT_PWM02		OUT_CCP3
#define OUT_PWM03		OUT_CCP4
#define OUT_PWM04		OUT_CCP5
#define OUT_PWM05		LATEbits.LATE0
#define OUT_PWM06		LATEbits.LATE1
#define OUT_PWM07		LATEbits.LATE2
#define OUT_PWM08		LATEbits.LATE3

// aliases for serial port two
#define usart2_TX		LATGbits.LATG1
#define usart2_RX		PORTGbits.RG2

// aliases for user modes
#define autonomous_mode		rxdata.rc_mode_byte.mode.autonomous
#define disabled_mode 		rxdata.rc_mode_byte.mode.disabled


//
// structure definitions
//

//
// MPLAB C18 _cinit table structure
//
extern far rom struct
{
	unsigned short num_init;
	struct _init_entry
	{
		unsigned long from;
		unsigned long to;
		unsigned long size;
	}
  	entries[];
}_cinit;

//
// this structure contains important system status information
//
typedef struct
{
  unsigned int  :6;
  unsigned int  autonomous:1;    /* Autonomous enabled = 1, disabled = 0 */
  unsigned int  disabled:1;      /* Robot disabled = 1, enabled = 0 */
} modebits;

//
// this structure allows you to address individual bits of a byte
//
typedef struct
{
  unsigned int  bit0:1;
  unsigned int  bit1:1;
  unsigned int  bit2:1;
  unsigned int  bit3:1;
  unsigned int  bit4:1;
  unsigned int  bit5:1;
  unsigned int  bit6:1;
  unsigned int  bit7:1;
} bitid;

//
// this structure defines the contents of the data received from the master microprocessor
//
typedef struct  {     /* begin rx_data_record structure */
  unsigned char packet_num;
  union
  { 
    bitid bitselect;
    modebits mode;            /*rxdata.rc_mode_byte.mode.(autonomous|disabled)*/
    unsigned char allbits;    /*rxdata.rc_mode_byte.allbits*/
  } rc_mode_byte;
  union
  {
    bitid bitselect;          /*rxdata.rc_receiver_status_byte.bitselect.bit0*/
    unsigned char allbits;    /*rxdata.rc_receiver_status_byte.allbits*/
  } rc_receiver_status_byte;  
  unsigned char spare01,spare02,spare03; /*reserved - for future use*/ 

  unsigned char oi_analog01, oi_analog02, oi_analog03, oi_analog04;  /*rxdata.oi_analog01*/
  unsigned char oi_analog05, oi_analog06, oi_analog07, oi_analog08;         
  unsigned char oi_analog09, oi_analog10, oi_analog11, oi_analog12;
  unsigned char oi_analog13, oi_analog14, oi_analog15, oi_analog16;
  unsigned char reserve[9];  /*reserved - for future use*/
  unsigned char master_version; 
} rx_data_record;

typedef rx_data_record *rx_data_ptr;

//
// this structure defines the contents of the data transmitted to the master microprocessor
//
typedef struct  {     /*begin tx_data_record structure*/
  unsigned char spare01,spare02,spare03,spare04;  /*reserved - for future use*/ 

  unsigned char rc_pwm01, rc_pwm02, rc_pwm03, rc_pwm04;   /*txdata.rc_pwm01*/
  unsigned char rc_pwm05, rc_pwm06, rc_pwm07, rc_pwm08;
  unsigned char rc_pwm09, rc_pwm10, rc_pwm11, rc_pwm12;
  unsigned char rc_pwm13, rc_pwm14, rc_pwm15, rc_pwm16;

  unsigned char user_cmd;     /*reserved - for future use*/
  unsigned char cmd_byte1;    /*reserved - Don't use*/
  unsigned char pwm_mask;     /*<VEX> make sure you know how this works before changing*/
  unsigned char warning_code; /*reserved - Don't use*/
  unsigned char reserve[4];   /*reserved - for future use*/
  unsigned char error_code;   /*reserved - Don't use*/
  unsigned char packetnum;    /*reserved - Don't use*/
  unsigned char current_mode; /*reserved - Don't use*/
  unsigned char control;      /*reserved - Don't use*/
} tx_data_record;

typedef tx_data_record *tx_data_ptr;


//
// this structure defines some flags which are used by the system
//
typedef struct
{
  unsigned int  NEW_SPI_DATA:1;
  unsigned int  TX_UPDATED:1;
  unsigned int  FIRST_TIME:1;
  unsigned int  TX_BUFFSELECT:1;
  unsigned int  RX_BUFFSELECT:1;
  unsigned int  SPI_SEMAPHORE:1;
  unsigned int  :2;
} packed_struct;


//
// variable declarations
//
extern near char FPFLAGS;		// needed for C18 2.4
extern tx_data_record txdata;	// located in vex_xx.lib 
extern rx_data_record rxdata;	// located in vex_xx.lib 
extern packed_struct statusflag;// located in vex_xx.lib

//
// function prototypes for code in vex.c
//
void main(void);
void Interrupt_Vector_Low(void);
void Interrupt_Handler_Low(void);
void _entry(void);
void _startup(void);
void _do_cinit(void);
unsigned char Get_Num_ADC_Channels(void);
void Set_Num_ADC_Channels(unsigned char);
unsigned int Quick_ADC(unsigned char);
unsigned int Quick_ADC_to_mV(unsigned int adc);

//
// these routines reside in vex_xx.lib
//
void Interrupt_Vector_High(void);
void Interrupt_Handler_High (void);
void Initialize_Registers (void);
void IFI_Initialization (void);
void User_Proc_Is_Ready (void);
void Putdata(tx_data_ptr);
void Getdata(rx_data_ptr);
void Configuration_Check(tx_data_ptr);
void Setup_PWM_Output_Type(int,int,int,int);

#endif
