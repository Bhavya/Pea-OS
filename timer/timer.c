#include "../rtx/rtx_inc.h"
#include "../debug/dbug.h"
#include "../shared/define.h"
#include "../io/uart.h"
#include "../messaging/messaging.h"
#include "../memory/memory.h"
#include "timer.h"

CHAR crlf[] = "\r\n\0";	
CHAR TimeString[] = "00:00:00";
SINT32 Ticks = +8639000;
SINT32 Hours = 0;
SINT32 Minutes = 0;
SINT32 Seconds = 0;

VOID c_timer_handler( VOID )
{
    if( Ticks < 8640000 )
	{
		Ticks++;
	} 
	else 
	{
		Ticks = 0;
	}
    TIMER0_TER = 2;

	if(Ticks%100 == 0){
		decrement_delayed_mailbox();
	}
	return;
}

void wallclock(){
	void * msg;
	CHAR * s;
	while(1){
		if(current_command != W){
			msg = receive_message(NULL);
			release_memory_block(msg);
		}
		
		SINT32 TotalSeconds = Ticks/100;  //number of actual seconds		
		Hours = TotalSeconds / 3600; 	
		TotalSeconds = TotalSeconds % 3600;		
		Minutes = TotalSeconds / 60;
		TotalSeconds = TotalSeconds % 60;		
		Seconds = TotalSeconds;
	
		if( Hours < 10 )
		{
			TimeString[0] = '0';
			TimeString[1] = (CHAR)(48 + Hours);
		} 
		else 
		{
			TimeString[0] = (CHAR)(48 + Hours/10);
			TimeString[1] = (CHAR)(48 + Hours%10);
		}
		
		if( Minutes < 10 )
		{
			TimeString[3] = '0';
			TimeString[4] = (CHAR)(48 + Minutes);
		}
		else 
		{
			TimeString[3] = (CHAR)(48 + Minutes/10);
			TimeString[4] = (CHAR)(48 + Minutes%10);
		}
		
		if( Seconds < 10 )
		{
			TimeString[6] = '0';
			TimeString[7] = (CHAR)(48 + Seconds);
		} 
		else 
		{
			TimeString[6] = (CHAR)(48 + Seconds/10);
			TimeString[7] = (CHAR)(48 + Seconds%10);
		}
		if(command_flag == 0){
			uprintf((CHAR *)"\r");
			uprintf( TimeString );		
			sleep(1);		
		}	
		if(command_flag == 1 && current_command == W){
			msg = receive_message(NULL);
			s = read_message(msg);
			current_command = W;
			release_memory_block(msg);
		}	
	}			
}

int set_system_time(int hh, int mm, int ss){
	Ticks = (hh*3600 + mm*60 + ss)*100;
	return 0;
}