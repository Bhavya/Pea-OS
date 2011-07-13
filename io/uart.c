#include "../rtx/rtx_inc.h"
#include "../debug/dbug.h"
#include "../shared/define.h"
#include "uart.h"
#include "../messaging/messaging.h"
#include "../memory/memory.h"

CHAR crlfgt[] = "\r\n>\0";	

void store_message( CHAR msg_char ){
	*(CHAR *)(kcd_msg + msg_length) = msg_char;
	msg_length++;
	return;
}

void clear_message(){
	int i = 0;
	while( i <= msg_length){
		*(CHAR *)(kcd_msg + i) = 0x00;
		i++;
	}
	msg_length = 0;
	return;
}

void uprintf( CHAR * string ){
	BYTE interrupt_status;
	int i = 0; 
	while( string[i] != '\0'){
		interrupt_status = SERIAL1_USR;
		while (!(interrupt_status & 4))
		{
			interrupt_status = SERIAL1_USR;
		}
		SERIAL1_WD = string[i];
		i++;
		SERIAL1_IMR = 2;
		interrupt_status = 0x00;
	}	
	return;	
}

void uprintn( int num, int length ){
	CHAR string[2];
	string[0] = (num + 48);
	string[1] = '\0';
	
	BYTE interrupt_status;
	int i = 0; 
	while( string[i] != '\0'){
		interrupt_status = SERIAL1_USR;
		while (!(interrupt_status & 4))
		{
			interrupt_status = SERIAL1_USR;
		}
		SERIAL1_WD = string[i];
		i++;
		SERIAL1_IMR = 2;
		interrupt_status = 0x00;
	}	
	return;	
}

VOID c_serial_handler( VOID )
{
	atomic_up();
	
	CharIn = '\0';
	BYTE interrupt_status;
    interrupt_status = SERIAL1_USR;    
	SERIAL1_IMR = 3;
	
    if( interrupt_status & 1 )
    {
		CharIn = SERIAL1_RD;
		CharOut = CharIn;
    }  
	while (!(interrupt_status & 4))
	{
		interrupt_status = SERIAL1_USR;
	}
	SERIAL1_IMR = 2;  
    if ( interrupt_status & 4 )
    {
		switch(CharIn){
			case '\r':
				uprintf(crlfgt);
				if(command_flag == 1){
					store_message((CHAR)'\0');
					void * p = request_memory_block();
					send_message(KCD_PID,write_message(p, kcd_msg));
					command_flag = 0;
					clear_message();
				}
				break;
			case '%':
				SERIAL1_WD = '%';
				command_flag = 1;	
				break;
			case '!':
				SERIAL1_WD = CharOut;
				display_queue_all();	
				sleep(5);
				break;
			case '@':
				SERIAL1_WD = CharOut;
				task_manager();	
				sleep(5);
				break;
			case '#':
				break;
			case '$':
				break;
			default:
				if(command_flag == 1){
					store_message((CHAR)CharOut);
				}
				SERIAL1_WD = CharOut;
				break;
		}
    }  	      
	
	atomic_down();
    return;
}

command_key * register_command( CHAR * key ){
	init_key_tail = (command_key *)malloc(sizeof(command_key));
	*(init_key_tail->keystroke) = *key;
	init_key_tail->next_key = NULL;
	init_key_tail = init_key_tail->next_key;
	return init_key_tail;
}
