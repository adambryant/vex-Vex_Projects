/*******************************************************************************
*
*	TITLE:		vex.c 
*
*	VERSION:	0.1 (Beta)                           
*
*	DATE:		15-May-2010
*
*	AUTHOR:		Kevin Watson
*				vex@kevin.org
*
*	COMMENTS:	This file contains the startup and interrupt service routine
*				dispatch code for the Vex robot controller. 
*
*				This file is best viewed with tabs set to four.
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

#include "vex.h"
#include "serial_ports.h"
#include "interrupts.h"
#include "timers.h"
#include "user_code.h"
#include "pwm.h"
#include "adc.h"

unsigned char Num_ADC_Channels;
unsigned char Old_Port_B;

/*******************************************************************************
*
*	FUNCTION:		main()
*
*	PURPOSE:		Entry point for your C code.		
* 
*	CALLED FROM:	_startup()
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
* 
*	COMMENTS:
*
*******************************************************************************/
void main(void)
{
	// take a snapshot of port b before interrupts are enabled
	Old_Port_B = PORTB;

	// call the system initialization code which initializes
	// processor registers and the SPI communication channel
	// with the master processor
	IFI_Initialization();

	// send printf() output to serial port one.
	stdout_serial_port = SERIAL_PORT_ONE;

	// initialize the serial ports
	// (see serial_ports_readme.txt and serial_ports.c/.h)
	Init_Serial_Port_One(BAUD_115200);
	Init_Serial_Port_Two(BAUD_19200);

	// call the user initialization code
	User_Initialization();

	// initialize the CCP-based PWM hardware
	txdata.pwm_mask = 0b11110000;
	Initialize_PWM();

	// let the master processor know we're done initializing
	Putdata(&txdata);
	User_Proc_Is_Ready();

	statusflag.NEW_SPI_DATA = 0; 

	while(TRUE)
	{
		// because the MPLAB simulation tool can't simulate the
		// master/slave communication channel, we'll just pretend
		// new data is sent every loop
		#ifdef _SIMULATOR
    	statusflag.NEW_SPI_DATA = 1;
		#endif

		if (statusflag.NEW_SPI_DATA)
		{
			Getdata(&rxdata);
			User_Code();
			PWM(pwm01, pwm02, pwm03, pwm04);
			Putdata(&txdata);
		}

		User_Code_Spin();
	}
}

/*******************************************************************************
*
*	FUNCTION:		Interrupt_Vector_Low()
*
*	PURPOSE:		Installs the low priority interrupt code at the low
*					priority interrupt vector, which is a fixed place in
*					memory where the microcontroller will start executing
*					code when it detects an interrupt condition. Because
*					this place in memory, at address 0x818, is intended 
*					to contain only a very small amount of code, general
*					practice is to place a "goto" instruction here that
*					will point to the real interrupt handler somewhere else
*					in memory. More information on interrupts can be found
*					in the PIC18F8520 data sheet.
* 
*	CALLED FROM:	Called in response to a hardware generated interrupt
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:
* 
*******************************************************************************/
#pragma code Interrupt_Vector_Low = LOW_INT_VECTOR
void Interrupt_Vector_Low (void)
{
	_asm
	goto Interrupt_Handler_Low  // jump to InterruptHandlerLow(), below
	_endasm
}

#pragma code
/*******************************************************************************
*
*	FUNCTION:		Interrupt_Handler_Low()
*
*	PURPOSE:		Determines which individual interrupt handler
*					should be called, clears the interrupt flag and
*					then calls the interrupt handler.
* 
*	CALLED FROM:	Interrupt_Vector_Low()
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:		Before altering this code, make sure you understand
*					how interrupts work. Documentation can be found in 
*					the C18 user's manual, the PIC18F8520 data sheet, and
*					the included C18_ISR.pdf document.
*
*					It is assumed that you won't be reading or writing
*					to program memory in your interrupt service routines.
*					If this is not the case, you'll need to remove
*					TBLPTRU, TBLPTRH, TBLPTRL and TABLAT from the nosave
*					section below.
*
*					It is also assumed that you won't be using function
*					pointers in your interrupt service routines. If this
*					is not the case, you'll need to remove the PCLATH and
*					PCLATU entries from the nosave section below.
* 
*******************************************************************************/
#ifdef USE_ALTERNATE_TMPDATA
#pragma tmpdata low_isr_tmpdata
#endif
#ifdef USE_C18_30
#pragma interruptlow Interrupt_Handler_Low nosave=section(".tmpdata"),TBLPTRU,TBLPTRH,TBLPTRL,TABLAT,PCLATH,PCLATU
#endif
#ifdef USE_C18_24
#pragma interruptlow Interrupt_Handler_Low save=PROD,section("MATH_DATA"),section(".tmpdata")
#endif
void Interrupt_Handler_Low()     
{
	unsigned char Port_B;
	unsigned char Port_B_Delta;
                             
	if (PIR1bits.RC1IF && PIE1bits.RC1IE) // rx1 interrupt?
	{
		#ifdef ENABLE_SERIAL_PORT_ONE_RX
		Rx_1_ISR(); // call the rx1 interrupt handler (in serial_ports.c)
		#endif
	}                              
	else if (PIR3bits.RC2IF && PIE3bits.RC2IE) // rx2 interrupt?
	{
		#ifdef ENABLE_SERIAL_PORT_TWO_RX
		Rx_2_ISR(); // call the rx2 interrupt handler (in serial_ports.c)
		#endif
	}
	#ifdef ENABLE_ADC  
	else if (PIR1bits.ADIF && PIE1bits.ADIE) // ADC interrupt
	{
		PIR1bits.ADIF = 0; // clear the ADC interrupt flag
		ADC_ISR(); // call the ADC interrupt handler (in adc.c)
	}
	#endif
	#ifdef ENABLE_INT_1  
	else if (INTCON3bits.INT2IF && INTCON3bits.INT2IE) // external interrupt 1?
	{ 
		INTCON3bits.INT2IF = 0; // clear the interrupt flag
		Int_1_ISR(); // call the interrupt 1 handler (in interrupts.c or encoder.c)
	}
	#endif
	#ifdef ENABLE_INT_2 
	else if (INTCON3bits.INT3IF && INTCON3bits.INT3IE) // external interrupt 2?
	{
		INTCON3bits.INT3IF = 0; // clear the interrupt flag
		Int_2_ISR(); // call the interrupt 2 handler (in interrupts.c or encoder.c)
	}
	#endif
	#ifdef ENABLE_INT_3_6 
	else if (INTCONbits.RBIF && INTCONbits.RBIE) // external interrupts 3 through 6?
	{
		Port_B = PORTB; // remove the "mismatch condition" by reading port b            
		INTCONbits.RBIF = 0; // clear the interrupt flag
		Port_B_Delta = Port_B ^ Old_Port_B; // determine which bits have changed
		Old_Port_B = Port_B; // save a copy of port b for next time around
	 
		#ifdef ENABLE_INT_3
		if(Port_B_Delta & 0x10) // did external interrupt 3 change state?
		{
			Int_3_ISR(Port_B & 0x10 ? 1 : 0); // call the interrupt 3 handler (in interrupts.c or encoder.c)
		}
		#endif
		#ifdef ENABLE_INT_4
		if(Port_B_Delta & 0x20) // did external interrupt 4 change state?
		{
			Int_4_ISR(Port_B & 0x20 ? 1 : 0); // call the interrupt 4 handler (in interrupts.c or encoder.c)
		}
		#endif
		#ifdef ENABLE_INT_5
		if(Port_B_Delta & 0x40) // did external interrupt 5 change state?
		{
			Int_5_ISR(Port_B & 0x40 ? 1 : 0); // call the interrupt 5 handler (in interrupts.c or encoder.c)
		}
		#endif
		#ifdef ENABLE_INT_6
		if(Port_B_Delta & 0x80) // did external interrupt 6 change state?
		{
			Int_6_ISR(Port_B & 0x80 ? 1 : 0); // call the interrupt 6 handler (in interrupts.c or encoder.c)
		}
		#endif
	}
	#endif
	#ifdef ENABLE_TIMER_0
	else if (INTCONbits.TMR0IF && INTCONbits.TMR0IE) // timer 0 interrupt?
	{
		INTCONbits.TMR0IF = 0; // clear the timer 0 interrupt flag
		Timer_0_ISR(); // call the timer 0 interrupt handler (in timers.c)
	}
	#endif
	#ifdef ENABLE_TIMER_1
	else if (PIR1bits.TMR1IF && PIE1bits.TMR1IE) // timer 1 interrupt?
	{
		PIR1bits.TMR1IF = 0; // clear the timer 1 interrupt flag
		Timer_1_ISR(); // call the timer 1 interrupt handler (in timers.c)
	}
	#endif
	#ifdef ENABLE_TIMER_2  
	else if (PIR1bits.TMR2IF && PIE1bits.TMR2IE) // timer 2 interrupt?
	{
		PIR1bits.TMR2IF = 0; // clear the timer 2 interrupt flag
		Timer_2_ISR(); // call the timer 2 interrupt handler (in timers.c)
	}
	#endif
	#ifdef ENABLE_TIMER_3  
	else if (PIR2bits.TMR3IF && PIE2bits.TMR3IE) // timer 3 interrupt?
	{
		PIR2bits.TMR3IF = 0; // clear the timer 3 interrupt flag
		Timer_3_ISR(); // call the timer 3 interrupt handler (in timers.c)
	}
	#endif
	#ifdef ENABLE_TIMER_4  
	else if (PIR3bits.TMR4IF && PIE3bits.TMR4IE) // timer 4 interrupt?
	{
		PIR3bits.TMR4IF = 0; // clear the timer 4 interrupt flag
		Timer_4_ISR(); // call the timer 4 interrupt handler (in timers.c)
	}
	#endif 
	else if (PIR1bits.TX1IF && PIE1bits.TX1IE) // tx1 interrupt?
	{
		#ifdef ENABLE_SERIAL_PORT_ONE_TX
		Tx_1_ISR(); // call the tx1 interrupt handler (in serial_ports.c)
		#endif
	}                              
	else if (PIR3bits.TX2IF && PIE3bits.TX2IE) // tx2 interrupt?
	{
		#ifdef ENABLE_SERIAL_PORT_TWO_TX
		Tx_2_ISR(); // call the tx2 interrupt handler (in serial_ports.c)
		#endif
	}
}
#ifdef USE_ALTERNATE_TMPDATA
#pragma tmpdata
#endif


/*******************************************************************************
*
*	FUNCTION:		_entry()
*
*	PURPOSE:		Installs the bootstrap code at location 0x800, which
*					is a fixed location in program memory where the boot-
*					loader expects the startup code to be found. Because
*					this place in memory, at address 0x800, is intended 
*					to contain only a very small amount of code, general
*					practice is to place a "goto" instruction here that
*					will point to the real boot code somewhere else in
*					memory.
* 
*	CALLED FROM:	Called by the bootloader after a reset
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:		This code is based upon version 1.7 of Microchip's
*					MPLAB-C18 startup code.
* 
*******************************************************************************/
#pragma code _entry_scn = RESET_VECTOR
void _entry (void)
{
	_asm
	goto _startup
	_endasm
}

/*******************************************************************************
*
*	FUNCTION:		_startup()
*
*	PURPOSE:		
* 
*	CALLED FROM:	_entry()
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
* 
*	COMMENTS:		This code is based upon version 1.7 of Microchip's
*					MPLAB-C18 startup code.
*
*******************************************************************************/
#pragma code _startup_scn
void _startup (void)
{
	_asm
    // Initialize the stack pointer
    lfsr 1, _stack
    lfsr 2, _stack
    clrf TBLPTRU, 0		// 1st silicon doesn't do this on POR
    #ifdef USE_C18_30
    bcf __FPFLAGS,6,0	// Initialize rounding flag for floating point libs
	#endif
	#ifdef USE_C18_24
	bcf FPFLAGS,6,0		// Initialize rounding flag for floating point libs
	#endif

    _endasm

    // Set all of memory to zeroes
    // Use FSR0 to decrement through memory from the top of bank 14 down to address 0x0
    _asm
	movlw 0xFF
	movwf FSR0L, 0
	movlw 0x0E
	movwf FSR0H, 0

	// Use memory location 0x0 as a indicator when we have reached the end
	movlw 1
	movwf 0x0, 0

	// Load WREG with the value to be stored throughout memory (0)
	movlw 0

	clear_loop:
	movwf POSTDEC0, 0
	movf  0x0,1,0
	bnz   clear_loop
	_endasm

	// initialize variables
    _do_cinit();

	loop:

	// Call the user's main routine
	main ();

	goto loop;
}

/*******************************************************************************
*
*	FUNCTION:		_do_cinit()
*
*	PURPOSE:		Initializes C variables		
* 
*	CALLED FROM:	_startup()
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
* 
*	COMMENTS:		This code is based upon version 1.7 of Microchip's
*					MPLAB-C18 startup code.
*
*******************************************************************************/
#pragma code _cinit_scn
void _do_cinit (void)
{
	// we'll make the assumption in the following code that these statics
	// will be allocated into the same bank.
	static short long prom;				// flash memory source address
	static unsigned short curr_byte;	
	static unsigned short curr_entry;
	static short long data_ptr;

	// get the number of entries in the _cinit initialization table
	// and save in the curr_entry variable
	TBLPTR = (short long)&_cinit;
	_asm
	movlb data_ptr
	tblrdpostinc
	movf TABLAT,0,0
	movwf curr_entry,1
	tblrdpostinc
	movf TABLAT, 0, 0
	movwf curr_entry+1,1
	_endasm

	// check to see if we have any (more) entries to process?
zero_test:
    _asm
	bnz get_entry
	tstfsz curr_entry,1
	bra get_entry
	_endasm
	goto done;

	// At this point we know that the table pointer points to the top
	// of the current entry in _cinit, so we can just start reading the
	// from, to, and size values...
get_entry:
	_asm
	tblrdpostinc		// read the low byte of the source address into the prom variable
	movf TABLAT,0,0
	movwf prom, 1
	tblrdpostinc		// read the middle byte of the source address into the prom variable
	movf TABLAT,0,0
	movwf prom+1,1
	tblrdpostinc		// read the high byte of the source address into the prom variable
	movf TABLAT,0,0
	movwf prom+2,1
	tblrdpostinc		// skip a byte since the source addresss is stored as a 32-bit value
	tblrdpostinc		// read the lower byte of the destination address directly into FSR0L
	movf TABLAT,0,0
	movwf FSR0L,0
	tblrdpostinc		// read the upper byte of the destination address directly into FSR0H
	movf TABLAT,0,0
	movwf FSR0H,0
	tblrdpostinc		// skip the next two bytes since the destination address is stored as a 32-bit value
	tblrdpostinc
	tblrdpostinc		// read the lower byte of the size and store in the lower byte of curr_byte
	movf TABLAT,0,0
	movwf curr_byte,1
	tblrdpostinc		// read the upper byte of the size and store in the upper byte of curr_byte
	movf TABLAT,0,0
	movwf curr_byte+1,1
	tblrdpostinc		// skip the next two bytes since the size is stored as a 32-bit value
	tblrdpostinc
	_endasm
  
	data_ptr = TBLPTR;	// save the table pointer because it now points to the next table entry and we'll need it later
	TBLPTR = prom;		// now assign the source address to the table pointer

	// determine if we have any more data to copy from this block
	_asm
	movlb curr_byte		
	movf curr_byte,1,1
copy_loop:
	bnz copy_one_byte 
	movf curr_byte+1,1,1
	bz done_copying

	// we've read the table entry and know that we have data to copy
	// so now do the actual transfer from flash memory to SRAM
copy_one_byte:
	tblrdpostinc
	movf TABLAT,0,0
	movwf POSTINC0,0
	decf curr_byte,1,1	// decrement and test the byte counter 
	bc copy_loop
	decf curr_byte+1,1,1
	bra copy_one_byte			
done_copying:
	_endasm

	// restore the table pointer for the next entry
	TBLPTR = data_ptr;
	// next entry...
	curr_entry--;
	goto zero_test;
done:
;
}
/*******************************************************************************
*
*	FUNCTION:		Set_Num_ADC_Channels()
*
*	PURPOSE:		Sets the number of analog channels		
* 
*	CALLED FROM:	user_code.c/User_Initialization
*
*	PARAMETERS:		Unsigned char with number of analog channels
*
*	RETURNS:		Nothing
* 
*	COMMENTS:
*
*******************************************************************************/
void Set_Num_ADC_Channels(unsigned char config)
{
	Num_ADC_Channels = config;
}

/*******************************************************************************
*
*	FUNCTION:		Get_Num_ADC_Channels()
*
*	PURPOSE:		Gets the number of analog channels		
* 
*	CALLED FROM:
*
*	PARAMETERS:		Nine.
*
*	RETURNS:		Unsigned char with number of analog channels.
* 
*	COMMENTS:
*
*******************************************************************************/
unsigned char Get_Num_ADC_Channels(void)
{
	return(Num_ADC_Channels);
}

/*******************************************************************************
*
*	FUNCTION:		Quick_ADC()
*
*	PURPOSE:		Does a single quick analog to digital conversion.
*
*	CALLED FROM:
*
*	PARAMETERS:		Unsigned char containing a value between one and
*					sixteen indicating the desired ADC channel.
*
*	RETURNS:		Unsigned int containing a value between 0 and 1024.
*
*	COMMENTS:		This software will work with all 2004-2008 IFI
*					robot controllers.
*
*					This software is not compatible with the software
*					found in adc.c/.h.
*
*******************************************************************************/
unsigned int Quick_ADC(unsigned char channel)  
{
	unsigned char adcon0_temp;
	unsigned int adc;
	unsigned char i;

	// decrement the channel number because channel
	// numbers start at zero, not one
	channel--;

	// make sure the ADC interrupt is disabled
	PIE1bits.ADIE = 0;

	// enable all sixteen analog inputs
	ADCON1 = 0;

	// select Fosc/64 as the ADC Conversion Clock, right justify data
	// in the conversion result register and select 20 Tad acquisition
	// period
	ADCON2 = 0b10111110;

	// Select the ADC channel. This also starts the process whereby the ADC 
	// sample and hold capacitor is allowed to start charging, which must be
	// completed before the analog to digital conversion can be started.
	adcon0_temp = channel;
	adcon0_temp <<= 2;
	adcon0_temp |= 0b00000001;
	ADCON0 = adcon0_temp;

	// a short delay to allow the ADC holding capacitor to charge
	for(i=0; i<20; i++);

	// start a new analog to digital conversion
	ADCON0bits.GO = 1;

	// wait for the conversion to finish
	while(ADCON0bits.GO == 1);

	// get the conversion result
	adc = ADRESH;
	adc <<= 8;
	adc += ADRESL;

	// turn off the ADC hardware
	ADCON0 = 0;

	return(adc);
}

/*******************************************************************************
*
*	FUNCTION:		Quick_ADC_to_mV()
*
*	PURPOSE:		Converts the raw output of the Quick_ADC()
*					function to millivolts.
*
*	CALLED FROM:
*
*	PARAMETERS:		Quick_ADC() output value to convert
*
*	RETURNS:		Unsigned int containing the value in millivolts
*
*	COMMENTS:
*
*******************************************************************************/
unsigned int Quick_ADC_to_mV(unsigned int adc)
{
	return((unsigned int)(((long)adc*625L)/128L));
}
