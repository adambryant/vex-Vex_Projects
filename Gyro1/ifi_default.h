/*******************************************************************************
* FILE NAME: ifi_default.h
*
* DESCRIPTION: 
*  This file contains important data definitions.              
*
*  DO NOT EDIT THIS FILE!
*******************************************************************************/

#ifndef __ifi_default_h_
#define __ifi_default_h_

#include "ifi_picdefs.h"

#ifdef UNCHANGEABLE_DEFINITION_AREA

/*******************************************************************************
                             ALIAS DEFINITIONS
*******************************************************************************/
                           /* DO NOT CHANGE! */

#define   DATA_SIZE          30
#define   SPI_TRAILER_SIZE   2
#define   SPI_XFER_SIZE      DATA_SIZE + SPI_TRAILER_SIZE

#define   RESET_VECTOR      0x800
#define   HIGH_INT_VECTOR   0x808
#define   LOW_INT_VECTOR    0x818


/*******************************************************************************
                            VARIABLE DECLARATIONS
*******************************************************************************/
                            /* DO NOT CHANGE! */
/* This structure contains important system status information. */
typedef struct
{
  unsigned int  :5;
  unsigned int  user_display:1;  /* User display enabled = 1, disabled = 0 */
  unsigned int  autonomous:1;    /* Autonomous enabled = 1, disabled = 0 */
  unsigned int  disabled:1;      /* Competition enabled = 1, disabled = 0 */
} modebits;

/******************************************************************************/
                            /* DO NOT CHANGE! */
/* This structure allows you to address specific bits of a byte.*/
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

/******************************************************************************/
                            /* DO NOT CHANGE! */
/* This structure defines the contents of the data received from the Master
 * microprocessor. */
typedef struct  {     /* begin rx_data_record structure */
  unsigned char packet_num;
  union
  { 
    bitid bitselect;
    modebits mode;            /*rxdata.rc_mode_byte.mode.(user_display|autonomous|disabled)*/
    unsigned char allbits;    /*rxdata.rc_mode_byte.allbits*/
  } rc_mode_byte;
  union
  {
    bitid bitselect;          /*rxdata.oi_swA_byte.bitselect.bit0*/
    unsigned char allbits;    /*rxdata.oi_swA_byte.allbits*/
  } oi_swA_byte;  
  union
  {
    bitid bitselect;          /*rxdata.oi_swB_byte.bitselect.bit0*/
    unsigned char allbits;    /*rxdata.oi_swB_byte.allbits*/
  } oi_swB_byte;  
  union
  {
    bitid bitselect;          /*rxdata.rc_swA_byte.bitselect.bit0*/
    unsigned char allbits;    /*rxdata.rc_swA_byte.allbits*/
  } rc_swA_byte;
  union
  {
    bitid bitselect;          /*rxdata.rc_swB_byte.bitselect.bit0*/
    unsigned char allbits;    /*rxdata.rc_swB_byte.allbits*/
  } rc_swB_byte;
  unsigned char oi_analog01, oi_analog02, oi_analog03, oi_analog04;  /*rxdata.oi_analog01*/
  unsigned char oi_analog05, oi_analog06, oi_analog07, oi_analog08;         
  unsigned char oi_analog09, oi_analog10, oi_analog11, oi_analog12;
  unsigned char oi_analog13, oi_analog14, oi_analog15, oi_analog16;
  unsigned char rc_main_batt, rc_backup_batt;
  unsigned char reserve[8];
} rx_data_record;

typedef rx_data_record *rx_data_ptr;

/******************************************************************************/
                            /* DO NOT CHANGE! */
/* This structure defines the contents of the data transmitted to the master  
 * microprocessor. */
typedef struct  {     /*begin tx_data_record structure*/
  union
  { 
    bitid bitselect;          /*txdata.LED_byte1.bitselect.bit0*/
    unsigned char data;       /*txdata.LED_byte1.data*/
  } LED_byte1;
  union
  { 
    bitid bitselect;          /*txdata.LED_byte2.bitselect.bit0*/
    unsigned char data;       /*txdata.LED_byte2.data*/
  } LED_byte2;
  union
  {
    bitid bitselect;          /*txdata.user_byte1.bitselect.bit0*/
    unsigned char allbits;    /*txdata.user_byte1.allbits*/
  } user_byte1;               /*for OI feedback*/
  union
  {
    bitid bitselect;          /*txdata.user_byte2.bitselect.bit0*/
    unsigned char allbits;    /*txdata.user_byte2.allbits*/
  } user_byte2;               /*for OI feedback*/
  unsigned char rc_pwm01, rc_pwm02, rc_pwm03, rc_pwm04;   /*txdata.rc_pwm01*/
  unsigned char rc_pwm05, rc_pwm06, rc_pwm07, rc_pwm08;
  unsigned char rc_pwm09, rc_pwm10, rc_pwm11, rc_pwm12;
  unsigned char rc_pwm13, rc_pwm14, rc_pwm15, rc_pwm16;

  unsigned char user_cmd;     /*reserved - for future use*/
  unsigned char cmd_byte1;    /*reserved - Don't use*/
  unsigned char pwm_mask;     /*<EDU> make sure you know how this works before changing*/
  unsigned char warning_code; /*reserved - Don't use*/
  unsigned char user_byte3;   /*for OI feedback*/
  unsigned char user_byte4;   /*for OI feedback*/
  unsigned char user_byte5;   /*for OI feedback*/
  unsigned char user_byte6;   /*for OI feedback*/
  unsigned char error_code;   /*reserved - Don't use*/
  unsigned char packetnum;    /*reserved - Don't use*/
  unsigned char current_mode; /*reserved - Don't use*/
  unsigned char control;      /*reserved - Don't use*/
} tx_data_record;

typedef tx_data_record *tx_data_ptr;


/******************************************************************************/
                            /* DO NOT CHANGE! */
/* This structure defines some flags which are used by the system. */

typedef struct
{
  unsigned int  NEW_SPI_DATA:1;
  unsigned int  TX_UPDATED:1;
  unsigned int  FIRST_TIME:1;
  unsigned int  TX_BUFFSELECT:1;
  unsigned int  RX_BUFFSELECT:1;
  unsigned int  :3;
} packed_struct;

/******************************************************************************/

extern tx_data_record txdata; 
extern rx_data_record rxdata; 
extern packed_struct statusflag;

#else
#error  *** Error - Invalid Default File!
#endif

/*******************************************************************************
                           FUNCTION PROTOTYPES
*******************************************************************************/

/* These routines reside in ifi_library.lib */
void InterruptHandlerHigh (void);
void Initialize_Registers (void);
void IFI_Initialization (void);
void User_Proc_Is_Ready (void);
void Putdata(tx_data_ptr ptr);
void Getdata(rx_data_ptr ptr);
void Clear_SPIdata_flag(void);
void Setup_PWM_Output_Type(int pwmSpec1,int pwmSpec2,int pwmSpec3,int pwmSpec4);

#endif

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
