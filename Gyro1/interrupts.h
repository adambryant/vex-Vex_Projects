/*******************************************************************************
*
*	TITLE:		interrupts.h 
*
*	VERSION:	0.3 (Beta)                           
*
*	DATE:		15-May-2010
*
*	AUTHOR:		Kevin Watson
*				vex@kevin.org
*
*	COMMENTS:	This file contains template interrupt initialization & handling
*				code for the Vex robot controller.
*
*               This file best viewed with tabs set to four.
*
*	LEGALESE:	This software is licensed under the simplified BSD license,
*				a copy of which is included with the original distribution.
*
*				Copyright �2004-2010 R. Kevin Watson. All rights are reserved.
*
********************************************************************************
*
*	CHANGE LOG:
*
*	DATE         REV  DESCRIPTION
*	-----------  ---  ----------------------------------------------------------
*	22-Dec-2003  0.1  RKW Original
*	25-Feb-2004  0.2  RKW - Added the ability to clear the interrupt flag before
*	                  enabling the interrupt.
*	03-Jan-2008  0.3  RKW - Renamed all ISRs for consistancy across all
*	                  modules of the new robot controller code.
*	15-May-2010  0.3  RKW - Verified code can be built with Microchip C18 3.35
*
*******************************************************************************/
#ifndef _interrupts_h
#define _interrupts_h

// Remove the comment slashes from one or more of the following lines to
// enable the respective external interrupt(s). By doing so, you only
// enable the code within interrupts.c to become part of your software
// build. For your software to be fully functional, you must also enable
// the interrupt(s) in vex.h.
// #define ENABLE_INT_1_ISR
// #define ENABLE_INT_2_ISR
// #define ENABLE_INT_3_ISR
// #define ENABLE_INT_4_ISR
// #define ENABLE_INT_5_ISR
// #define ENABLE_INT_6_ISR

//
// If you modify stuff below this line, you'll break the software.
//

// #define ENABLE_INT_3_6_ISR if external interrupt 3, 4, 5 or 6 is enabled
#ifdef ENABLE_INT_3_ISR
#ifndef ENABLE_INT_3_6_ISR
#define ENABLE_INT_3_6_ISR
#endif
#endif
#ifdef ENABLE_INT_4_ISR
#ifndef ENABLE_INT_3_6_ISR
#define ENABLE_INT_3_6_ISR
#endif
#endif
#ifdef ENABLE_INT_5_ISR
#ifndef ENABLE_INT_3_6_ISR
#define ENABLE_INT_3_6_ISR
#endif
#endif
#ifdef ENABLE_INT_6_ISR
#ifndef ENABLE_INT_3_6_ISR
#define ENABLE_INT_3_6_ISR
#endif
#endif

// function prototypes
void Initialize_Int_1(void);	// initializes interrupt 1
void Initialize_Int_2(void);	// initializes interrupt 2
void Initialize_Int_3_6(void);	// initializes interrupts 3 through 6
void Int_1_ISR(void);			// external interrupt 1 service routine
void Int_2_ISR(void);			// external interrupt 2 service routine
void Int_3_ISR(unsigned char);	// external interrupt 3 service routine
void Int_4_ISR(unsigned char);	// external interrupt 4 service routine
void Int_5_ISR(unsigned char);	// external interrupt 5 service routine
void Int_6_ISR(unsigned char);	// external interrupt 6 service routine

#endif
