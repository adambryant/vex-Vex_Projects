/*******************************************************************************
*
*	TITLE:		timers.h 
*
*	VERSION:	0.2 (Beta)                           
*
*	DATE:		15-May-2010
*
*	AUTHOR:		Kevin Watson
*				vex@kevin.org
*
*	COMMENTS:	This file contains template timer initialization & interrupt
*				handling code for the Vex robot controller.
*
*               This file best viewed with tabs set to four.
*
*	LEGALESE:	This software is licensed under the simplified BSD license,
*				a copy of which is included with the original distribution.
*
*				Copyright ©2007-2010 R. Kevin Watson. All rights are reserved.
*
********************************************************************************
*
*	Change log:
*
*	DATE         REV  DESCRIPTION
*	-----------  ---  ----------------------------------------------------------
*	24-Dec-2007  0.1  RKW Original
*	03-Jan-2008  0.2  RKW - Renamed all ISRs for consistancy across all
*	                  modules of the new robot controller code.
*	15-May-2010  0.2  RKW - Verified code can be built with Microchip C18 3.35
*
*******************************************************************************/
#ifndef _timers_h
#define _timers_h

// Remove the comment slashes from one or more of the following lines
// to enable the respective timer(s). By doing so, you only enable the
// code within timers.c to become part of your software build. For your
// software to be fully functional, you must also enable the timer(s)
// in vex.h.
// #define ENABLE_TIMER_0_ISR // unused
// #define ENABLE_TIMER_1_ISR // unused
   #define ENABLE_TIMER_2_ISR // enable if using the millisecond time clock
   #define ENABLE_TIMER_3_ISR // enable if used the PWM software
// #define ENABLE_TIMER_4_ISR // enabling here will conflict with ADC

//
// If you modify stuff below this line, you'll break the software.
//

// function prototypes
void Initialize_Timer_0(void);	// initializes the timer 0 hardware
void Initialize_Timer_1(void);	// initializes the timer 1 hardware
void Initialize_Timer_2(void);	// initializes the timer 2 hardware
void Initialize_Timer_3(void);	// initializes the timer 3 hardware
void Initialize_Timer_4(void);	// initializes the timer 4 hardware
void Timer_0_ISR(void);			// timer 0 interrupt service routine
void Timer_1_ISR(void);			// timer 1 interrupt service routine
void Timer_2_ISR(void);			// timer 2 interrupt service routine
void Timer_3_ISR(void);			// timer 3 interrupt service routine
void Timer_4_ISR(void);			// timer 4 interrupt service routine
unsigned long Get_Time(void);
void Set_Time(unsigned long);

#endif
