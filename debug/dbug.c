/**
 * @file: dbug.c
 * @brief: output to janusROM terminal by using trap #15
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang
 * @date: 2010/05/03
 */

#include "dbug.h"
#include "../shared/define.h"
#include "../io/uart.h"

/**
 * @brief: C Function wrapper for TRAP #15 function to output a character
 * @param: c the charcter to output to janusROM  
 */

VOID rtx_dbug_out_char( CHAR c )
{
	
    /* Store registers */
    asm( "move.l %d0, -(%a7)" );
    asm( "move.l %d1, -(%a7)" );

    /* Load CHAR c into d1 */
    asm( "move.l 8(%a6), %d1" );  /* Standard Motorola syntax */ 

    /* Setup trap function */
    asm( "move.l #0x13, %d0" );
    asm( "trap #15" );

    /* Restore registers  */
    asm(" move.l (%a7)+, %d1" );
    asm(" move.l (%a7)+, %d0" );
}

/**
 * @brief: Prints a C-style null terminated string
 * @param: s the string to output to janusROM terminal 
 */
SINT32 rtx_dbug_outs( CHAR* s )
{
    if ( s == NULL )
    {
        return RTX_ERROR;
    }
    while ( *s != '\0' )
    {
        rtx_dbug_out_char( *s++ );
    }
    return RTX_SUCCESS;
}

SINT32 rtx_dbug_out_block( CHAR* s )
{
	int i = 0;
    while ( i < 128 )
    {
        rtx_dbug_out_char( *s++ );
		i++;
    }
    return RTX_SUCCESS;
}

VOID print_int(UINT32 value)
{
	asm("move.b #00, -(%a7)");
	do {
		CHAR cur_digit = 0x30 + (value % 10);
		value /= 10;
		asm("move.b %[digit], -(%%a7)" : : [digit] "g" (cur_digit));
	} while (value != 0);
	CHAR cur_char;
	do {
		asm("move.b (%%a7)+,%[digit]" : [digit] "=g" (cur_char));
		rtx_dbug_out_char(cur_char);
	} while (cur_char != '\0');
}

VOID rtx_dbug_outs_int(CHAR * s, UINT32 n)
{
	rtx_dbug_outs(s);
	print_int(n);
}

VOID exception(ERROR_CODE err){
	if(err == ERROR){
		uprintf((CHAR *)"\r\nERROR: Error.\r\n>");
	} else if(err == INVALID_PRIORITY){
		uprintf((CHAR *)"\r\nERROR: Invalid priority\r\n>");
	} else if(err == MSG_INVALID_PROCESS){
		uprintf((CHAR *)"\r\nERROR: Message sent to non-existent process.\r\n>");
	} else if(err == SET_PRIO_INVALID_PROCESS){
		uprintf((CHAR *)"\r\nERROR: Tried to change priority on non-existent or kernel-owned process.\r\n>");
	} else if(err == INVALID_HOTKEY){
		uprintf((CHAR *)"\r\nERROR: Invalid hotkey.\r\n>");
	} else if(err == INVALID_COMMAND){
		uprintf((CHAR *)"\r\nERROR: Bad command.\r\n>");
	} else if(err == INVALID_COMMAND_PARAMETER){
		uprintf((CHAR *)"\r\nERROR: Invalid command parameter.\r\n>");
	} else if(err == BLOCKED_ON_RECEIVE){
		uprintf((CHAR *)"\r\nERROR: Blocked on receive.\r\n>");
	} else if(err == BLOCKED_ON_MEM){
		uprintf((CHAR *)"\r\nERROR: Blocked on memory.\r\n>");
	} else if(err == OUT_OF_MEM){
		uprintf((CHAR *)"\r\nERROR: Out of memory.\r\n>");
	} else if(err == MAILBOX_FULL){
		uprintf((CHAR *)"\r\nERROR: Mailbox full. Please wait and try again.\r\n>");
	} else if(err == INVALID_TIMESTAMP){
		uprintf((CHAR *)"\r\nERROR: Invalid timestamp. Please use a hh:mm:ss format.\r\n 0 <= hh <= 23, 0 <= mm <= 59, 0 <= ss <= 59\r\n>");
	} else if(err == NON_EXISTENT_PROCESS){
		uprintf((CHAR *)"\r\nERROR: Non existent process referenced.\r\n>");
	} else if(err == CANNOT_RELEASE){
		uprintf((CHAR *)"\r\nERROR: Cannot release memory block.\r\n>");
	} else if(err == SET_PRIO_SAME_PRIORITY){
		uprintf((CHAR *)"\r\nERROR: Process already in this priority.\r\n>");
	} if(err == REFERENCE_TERMINATED_PROCESS){
		uprintf((CHAR *)"\r\nERROR: Terminated process referenced.\r\n>");
	} if(err == WALLCLOCK_RUNNING){
		uprintf((CHAR *)"\r\nERROR: Cannot run this command while wallclock is running. Please use %WS or %WT only.\r\n>");
	}
	return;
}

VOID task_manager(){
  pib * temp = init_blocks_head;
  pcb * temp2;
  rtx_dbug_outs((CHAR *)"Task Manager");
  rtx_dbug_outs((CHAR *)"\r\n__________________.");
  rtx_dbug_outs((CHAR *)"\r\n|  P  |  P  |  S  |");
  rtx_dbug_outs((CHAR *)"\r\n__________________.");
  while(temp != NULL){
    temp2 = temp->control_block;
    rtx_dbug_outs_int((CHAR *)"\r\n|  ", temp->process_ID);
    rtx_dbug_outs_int((CHAR *)"  |  ", temp2->process_priority);
    rtx_dbug_outs_int((CHAR *)"  |  ", temp2->process_state);
    rtx_dbug_outs((CHAR *)"  |");
    temp = temp->next_process;
  }
  rtx_dbug_outs((CHAR *)"\r\n__________________.\r\n");
  return;
}

VOID display_queue(state queue_state){
  int i;
	if(queue_state == READY_STATE){
    for(i=0;i<4;i++){
      rtx_dbug_outs((CHAR *)"ERROR CODE 6: BLOCKED ON MEMORY\r\n");
    }
	} else if(queue_state == BLOCKED_MSG_STATE){
	
	} else if(queue_state == BLOCKED_MEM_STATE){
	
	}
	return;
}

VOID display_queue_all(){
	pcb * temp;
	int i;
	for(i=0;i<4;i++){
		rtx_dbug_outs_int((CHAR *)"\r\n Ready Queue: Priority ", i);
		temp = ready_queue[i];
		while(temp != NULL){
			rtx_dbug_outs_int((CHAR *)"\r\n\tProcess ", temp->process_ID);
			temp = temp->next_process;
		}	
		rtx_dbug_outs((CHAR *)"\r\n"); 
    }
	
	for(i=0;i<4;i++){
	rtx_dbug_outs_int((CHAR *)"\r\n Blocked Msg Priority ", i);
	temp = bmsg_queue[i];
	while(temp != NULL){
		rtx_dbug_outs_int((CHAR *)"\r\n\tProcess ", temp->process_ID);
		temp = temp->next_process;
	}	
	rtx_dbug_outs((CHAR *)"\r\n"); 
    }
	
	for(i=0;i<4;i++){
	rtx_dbug_outs_int((CHAR *)"\r\n Blocked Mem Priority ", i);
	temp = bmem_queue[i];
	while(temp != NULL){
		rtx_dbug_outs_int((CHAR *)"\r\n\tProcess ", temp->process_ID);
		temp = temp->next_process;
	}	
	rtx_dbug_outs((CHAR *)"\r\n"); 
    }
	return;
}

VOID display_mailbox(){
	return;
}

VOID display_msg_log(){
	return;
}

VOID display_hotkeys(){

}