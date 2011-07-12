#include "../rtx/rtx_inc.h"
#include "../debug/dbug.h"
#include "../shared/define.h"
#include "messaging.h"
#include "../process_management/process_management.h"

int send_message (int process_ID, void * MessageEnvelope)
{
	pcb * pot_blocked;
	
	if(process_ID >= 1 || process_ID <= NUM_PROCESSES || process_ID == KCD_PID){
		int message = 0;
		while(mailbox[message].sender_ID != -1){
			message++;		
		}
		
		if(message > 31){
			return -1;
		} else {
			mailbox[message].sender_ID = running->process_ID;
			mailbox[message].destination_ID = process_ID;
			mailbox[message].msg_body = MessageEnvelope;
			red_lever++;
		}	
		
		if(red_lever > 0){
			pot_blocked = find_blocked_proc_by_id(process_ID);
			if(pot_blocked > 0x00){
				insert_process(remove_blocked_process(pot_blocked,BLOCKED_MSG_STATE), pot_blocked->process_priority);
			}
		}
		return message;
	} else {
		#ifdef _ERR_
			exception(MSG_INVALID_PROCESS);
		#endif
	}
	
	#ifdef _ERR_
		exception(ERROR);
	#endif
    return -1;
}

void * receive_message(int * sender_ID)
{
	#ifdef _DEBUG_
        //rtx_dbug_outs((CHAR *)"rtx: receive message\r\n");
	#endif

	pib * init_block;
	int message = 0;
	while(mailbox[message].destination_ID != running->process_ID){
		if(message > 31){
			break;
		} else {
			message++;	
		}
	}
	if(message < 32){	
	
		#ifdef _DEBUG_
			//rtx_dbug_outs((CHAR *)"rtx: message received\r\n");
		#endif		
		
		*sender_ID += mailbox[message].sender_ID;
		mailbox[message].sender_ID = -1;
		red_lever--;		
		return mailbox[message].msg_body;
	} else {
		init_block = init_blocks_head;
		while(init_block != NULL){
			if(init_block->process_ID == running->process_ID){
				insert_blocked_process(remove_process(init_block->control_block), running->process_priority, BLOCKED_MSG_STATE);
				release_processor();			
				message = 0;
				while(mailbox[message].destination_ID != running->process_ID){
					if(message > 31){
						break;
					} else {
						message++;	
					}
				}
				if(message < 32){	
					*sender_ID += mailbox[message].sender_ID;
					mailbox[message].sender_ID = -1;
					red_lever--;	
					return mailbox[message].msg_body;
				}
				return 0x00;
			} else {
				init_block = init_block->next_process;
			}
		}
	}
	return 0x00;
}

int delayed_send(int process_ID, void * MessageEnvelope, int delay)
{
    return 0;
}

void unblock_waiting_procs(){
	int message;
	pcb * temp;
	int i;
	for(i = 0; i < 4; i++){
		message = 0;
		temp = bmsg_queue[i];
		while(temp != NULL){
			message = 0;
			while(mailbox[message].destination_ID != temp->process_ID){
				if(message > 31){
					break;
				} else {
					message++;	
				}
			}
			if(message < 32){
				insert_process(remove_blocked_process(temp, BLOCKED_MSG_STATE),temp->process_priority);
				red_lever--;
			} else {
				temp = temp->next_process;
			}
		}
	}
	return;
}

int message_type(int i){
	return mailbox[i].msg_type;
}

void set_message_type(int i, int v){
	mailbox[i].msg_type = v;
	return;
}

void * write_message( void * memory_block, CHAR * msg ){
	int i=0;
	while(*(msg+i) != '\0'){
		*((CHAR *)(memory_block+64+i)) = *(msg+i);
		i++;
	}
	return memory_block;
}

CHAR * read_message( void * memory_block){
	CHAR * s = (CHAR *)(memory_block+64);
	return s;
}


