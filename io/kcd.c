#include "../rtx/rtx_inc.h"
#include "../debug/dbug.h"
#include "../shared/define.h"
#include "uart.h"
#include "../messaging/messaging.h"
#include "../memory/memory.h"
#include "../priority/priority.h"
#include "../timer/timer.h"

command_key * register_command( CHAR * key ){
	init_key_tail = (command_key *)malloc(sizeof(command_key));
	*(init_key_tail->keystroke) = *key;
	init_key_tail->next_key = NULL;
	init_key_tail = init_key_tail->next_key;
	return init_key_tail;
}

int find_command(CHAR * s){
	if(*(init_key_tail->keystroke) == *s){
		return RTX_SUCCESS;
	}
	return RTX_ERROR;
}

void kcd(){
	void * msg;
	void * p;
	int process = 0;
	int priority = 0;
	int result = 0;
	int hh = 0;
	int mm = 0;
	int ss = 0;
	while(1){
		msg = receive_message(NULL);
		CHAR * s = read_message(msg);
		switch(*s){
			case 'W':
				p = request_memory_block();
				switch(*(s+1)){
					case 'S':
						if((*(s+2)-48)>2 || (*(s+3)-48)>9 || (*(s+5)-48)>5 || (*(s+6)-48)>9 || (*(s+8)-48)>5 || (*(s+9)-48)>9){
							#ifdef _ERR_
								exception(INVALID_TIMESTAMP);
							#endif
						} else if(*(s+4) != ':' || *(s+7) != ':'){
							#ifdef _ERR_
								exception(INVALID_TIMESTAMP);
							#endif
						} else {
							hh = (*(s+2)-48)*10;
							hh += (*(s+3)-48);
							mm = (*(s+5)-48)*10;
							mm += (*(s+6)-48);
							ss = (*(s+8)-48)*10;
							ss += (*(s+9)-48);
							set_system_time(hh,mm,ss);
							send_message(WALLCLOCK_PID, p);
							current_command = WS;
						}
						while(*s != 0x00 || *s != '\0'){
							*s = 0x00;
							s++;
						}
						break;
					case 'T':
						timer_display = 0;
						uprintf((CHAR *)"WALLCLOCK TERMINATED\r\n>");
						current_command = WT;
						break;
					default:
						timer_display = 1;
						send_message(WALLCLOCK_PID, write_message(p,(CHAR*)"W"));
						current_command = W;
						break;
				}
				while(*s != 0x00 || *s != '\0'){
					*s = 0x00;
					s++;
				}
				break;
			case 'C':
				if(timer_display == 0){
					current_command = C;
					process = (*(s+2) - 48);
					priority = (*(s+4) - 48);
					if(*(s+3) - 48 >= 0){
						#ifdef _ERR_
							exception(SET_PRIO_INVALID_PROCESS);
						#endif
					} else if(*(s+5) - 48 >= 0){
						#ifdef _ERR_
							exception(INVALID_PRIORITY);
						#endif
					} else {
						uprintf((CHAR *)"Changing priorities\r\n");
						result = set_process_priority(process, priority);
						if(result == RTX_SUCCESS){
							uprintf((CHAR *)">Process ");
							uprintn(process, 1);
							uprintf((CHAR *)" changed to priority ");
							uprintn(priority, 1);
							uprintf((CHAR *)".\r\n>");
						}
					}
					while(*s != 0x00 || *s != '\0'){
						*s = 0x00;
						s++;
					}
				} else {
					#ifdef _ERR_
						exception(WALLCLOCK_RUNNING);
					#endif
				}
				break;
			case 'R':
				if(timer_display == 0){
					uprintf((CHAR *)"\r\nI am the Great Cornholio!\r\n");
				} else {
					#ifdef _ERR_
						exception(WALLCLOCK_RUNNING);
					#endif
				}
				break;
			default:
				if(timer_display == 0){
					if(find_command(s) == -1){
						current_command = OTHER;
						#ifdef _ERR_
							exception(INVALID_COMMAND);
						#endif
					} else {
						current_command = OTHER;
					}
				} else {
					#ifdef _ERR_
						exception(WALLCLOCK_RUNNING);
					#endif
				}
				break;
		}
		release_memory_block(msg);
	}
	return;
}