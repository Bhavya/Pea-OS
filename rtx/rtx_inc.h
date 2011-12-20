/*****************************************************************
 *
 *                           E&CE 354 RTX
 *                   (c)1998, All Rights Reserved
 *
 *                  Chris McKillop and  Craig Stout
 *                  [cdmckill,castout]@uwaterloo.ca
 *
 *****************************************************************/

/*  $Id: rtx_inc.h,v 1.1 2010/05/16 20:24:00 yqhuang Exp $ */

/******************************************************************
 *
 *  $Source: /usr/local/CVS_Repository/ece354/autoeval/new_greybox/dummy/rtx_inc.h,v $
 *  $Author: yqhuang $
 *  $Date: 2010/05/16 20:24:00 $
 *
 *  Purpose:   Base include file for the RTX
 *
 *  $Log: rtx_inc.h,v $
 *  Revision 1.1  2010/05/16 20:24:00  yqhuang
 *  *** empty log message ***
 *
 *  Revision 1.1  2010/05/03 21:32:25  yqhuang
 *  *** empty log message ***
 *
 *
 *  [LOG REMOVED]
 *
 ******************************************************************/

#if !defined( RTX_BASE_H__ )
#define RTX_BASE_H__

/******************************************************************
 *                      CONSTANTS
 ******************************************************************/

/*
 * Data Types by Size
 */
#define SINT32  signed long int
#define UINT32  unsigned long int
#define SINT16  signed short int
#define UINT16  unsigned short int
#define SINT8   signed char
#define UINT8   unsigned char
#define CHAR    signed char
#define BYTE    unsigned char
#define VOID    void
#define BOOLEAN signed long int


#define ESC                 0x1B
#define BKSP                '\b'
#define CR                  '\r'
#define LF                  '\n'


#if !defined( TRUE )
#define TRUE 1
#endif

#if !defined( FALSE )
#define FALSE 0
#endif

#if !defined( NULL )
#define NULL 0
#endif


/*
 * Coldfire system defines
 */
#define RTX_COLDFIRE_MBAR    (BYTE *)(0xF0000000)
#define SIM_IMR              *( (UINT32 *)( RTX_COLDFIRE_MBAR + 0x44 ) )


/*
 * Coldfire Timer Defines
 */
#define TIMER0_TMR  *( (UINT16 *)( RTX_COLDFIRE_MBAR + 0x140 ) )
#define TIMER0_TRR  *( (UINT16 *)( RTX_COLDFIRE_MBAR + 0x144 ) )
#define TIMER0_TCN  *( (UINT16 *)( RTX_COLDFIRE_MBAR + 0x14C ) )
#define TIMER0_ICR  *( RTX_COLDFIRE_MBAR + 0x04d )
#define TIMER0_TER  *( RTX_COLDFIRE_MBAR + 0x151 )

#define TIMER1_TMR  *( (UINT16 *)(RTX_COLDFIRE_MBAR + 0x180 ) )
#define TIMER1_TRR  *( (UINT16 *)(RTX_COLDFIRE_MBAR + 0x184 ) )
#define TIMER1_TCN  *( (UINT16 *)(RTX_COLDFIRE_MBAR + 0x18C ) )
#define TIMER1_ICR  *( RTX_COLDFIRE_MBAR + 0x04e )
#define TIMER1_TER  *( RTX_COLDFIRE_MBAR + 0x191 )


/*
 * Coldfire Serial Defines
 */
#define SERIAL1_UCR     *( RTX_COLDFIRE_MBAR + 0x208 )
#define SERIAL1_UBG1    *( RTX_COLDFIRE_MBAR + 0x218 )
#define SERIAL1_UBG2    *( RTX_COLDFIRE_MBAR + 0x21C )
#define SERIAL1_UCSR    *( RTX_COLDFIRE_MBAR + 0x204 )
#define SERIAL1_USR     *( RTX_COLDFIRE_MBAR + 0x204 )
#define SERIAL1_UMR     *( RTX_COLDFIRE_MBAR + 0x200 )
#define SERIAL1_ICR     *( RTX_COLDFIRE_MBAR + 0x51  )
#define SERIAL1_IVR     *( RTX_COLDFIRE_MBAR + 0x230 )
#define SERIAL1_ISR     *( RTX_COLDFIRE_MBAR + 0x214 )
#define SERIAL1_IMR     *( RTX_COLDFIRE_MBAR + 0x214 )
#define SERIAL1_RD      *( RTX_COLDFIRE_MBAR + 0x20C )
#define SERIAL1_WD      *( RTX_COLDFIRE_MBAR + 0x20C )


/*
 * RTX Error Codes
 */
#define RTX_SUCCESS 0
#define RTX_ERROR   -1

#endif
