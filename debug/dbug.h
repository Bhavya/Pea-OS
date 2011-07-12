/**
 * @file: dbug.h
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang  
 * @date: 2010/05/03
 * @brief: Header file of dbug.c.  
 */
#ifndef _DEBUG_
#define _DEBUG_
#endif

#ifndef _DBUG_H_
#define _DBUG_H_
#endif

#ifndef _DBUG_HOTKEYS_
#define _DBUG_HOTKEYS_
#endif

#ifndef _ERR_
#define _ERR_

#include "../rtx/rtx_inc.h"
enum ERROR_CODE
{
   	ERROR,
	INVALID_PRIORITY,
   	MSG_INVALID_PROCESS,
	SET_PRIO_INVALID_PROCESS,
   	INVALID_HOTKEY,
	INVALID_HOTKEY_PARAMETER,
	BLOCKED_ON_RECEIVE,
	BLOCKED_ON_MEM
};

typedef enum ERROR_CODE ERROR_CODE;

VOID rtx_dbug_out_char( CHAR c );   // output a char to janusROM terminal
SINT32 rtx_dbug_outs( CHAR * s );    // output a string to janusROM terminal
SINT32 rtx_dbug_out_block( CHAR* s );
VOID rtx_dbug_outs_int(CHAR *, UINT32);
VOID exception( ERROR_CODE );
#endif /* _DBUG_H_ */
