#include "../rtx/rtx_inc.h"
#include "../debug/dbug.h"
#include "../shared/define.h"
#include "process_management.h"
#include "../memory/memory.h"
#include "../messaging/messaging.h"

int release_processor()
{
    asm("TRAP #0");
	return 0;
}

void process_bootstrap( pcb * control_block ){
	proc_pointer = (control_block->process);
	(*proc_pointer)();
	remove_process(control_block);
	running = NULL;
	total_procs--;
	if(total_procs > 0){
		release_processor();
	}
	return;
}

int insert_process(pcb * control_block, UINT32 process_priority){
	pcb * current_process;
	if(process_priority < 0 || process_priority > 3){
		rtx_dbug_outs((CHAR *)"\r\n Process priority invalid value.");
		return -1;
	} else {
		control_block->process_priority = process_priority;
	}
	
	if(ready_queue[process_priority] != NULL)
	{
		current_process = ready_queue[process_priority];
		while(current_process->next_process != NULL){
			current_process = current_process->next_process;
		}
		control_block->process_state = READY_STATE;
		current_process->next_process = control_block;
		return 0;
	}
	else
	{
		ready_queue[process_priority] = control_block;
		return 0;
	} 
	
	return -1;
}

pcb * insert_new_process(pib * process){
	pcb * current_process;
	pcb * new_process = (pcb *)malloc(sizeof(pcb));
	
	new_process->process_ID = process->process_ID;
	new_process->process_priority = process->process_priority;
	new_process->process = process->process;
	new_process->stack_ptr = (UINT32)proc_malloc();
	new_process->next_process = NULL;
	new_process->process_state = NEW_STATE;
	
	if(ready_queue[new_process->process_priority] != NULL)
	{
		current_process = ready_queue[new_process->process_priority];
		while(current_process->next_process != NULL){
			current_process = current_process->next_process;
		}
		current_process->next_process = new_process;
	}
	else if(ready_queue[new_process->process_priority] == NULL)
	{
		ready_queue[new_process->process_priority] = new_process;

	} 
	
	return new_process;
}

pcb * remove_process(pcb * control_block){	
	pcb * queued_process;
	
	if(ready_queue[control_block->process_priority]->process_ID == control_block->process_ID){
		if(control_block->next_process != NULL){
			ready_queue[control_block->process_priority] = control_block->next_process;
		} else {
			ready_queue[control_block->process_priority] = NULL;
		}
	} else {
		queued_process = ready_queue[control_block->process_priority];
		while(queued_process->next_process != NULL){
			if(queued_process->next_process == control_block){
				queued_process->next_process = control_block->next_process;
				break;
			} else {
				queued_process = queued_process->next_process;
			}
		}
	}
	
	control_block->next_process = NULL;
	
	return control_block;
}

int insert_blocked_process(pcb * control_block, UINT32 process_priority, state type){
	pcb * current_process;
	if(process_priority < 0 || process_priority > 3){
		rtx_dbug_outs((CHAR *)"\r\n Process priority invalid value.");
		return -1;
	} else {
		control_block->process_priority = process_priority;
	}
	
	if(type == BLOCKED_MSG_STATE){
		if(bmsg_queue[process_priority] != NULL)
		{
			current_process = bmsg_queue[process_priority];
			while(current_process->next_process != NULL){
				current_process = current_process->next_process;
			}
			control_block->process_state = BLOCKED_MSG_STATE;
			current_process->next_process = control_block;
			return 0;
		}
		else if(bmsg_queue[process_priority] == NULL)
		{
			bmsg_queue[process_priority] = control_block;
			return 0;
		} 
	} else if(type == BLOCKED_MEM_STATE){
		if(bmem_queue[process_priority] != NULL)
		{
			current_process = bmem_queue[process_priority];
			while(current_process->next_process != NULL){
				current_process = current_process->next_process;
			}
			control_block->process_state = BLOCKED_MEM_STATE;
			current_process->next_process = control_block;
			return 0;
		}
		else if(bmem_queue[process_priority] == NULL)
		{
			bmem_queue[process_priority] = control_block;
			return 0;
		} 
	}	
	return -1;
}

pcb * remove_blocked_process(pcb * control_block, state type){	
	pcb * queued_process;
	
	if(type == BLOCKED_MSG_STATE){
		if(bmsg_queue[control_block->process_priority]->process_ID == control_block->process_ID){
			if(control_block->next_process != NULL){
				bmsg_queue[control_block->process_priority] = control_block->next_process;
			} else {
				bmsg_queue[control_block->process_priority] = NULL;
			}
		} else {
			queued_process = bmsg_queue[control_block->process_priority];
			while(queued_process->next_process != NULL){
				if(queued_process->next_process == control_block){
					queued_process->next_process = control_block->next_process;
					break;
				} else {
					queued_process = queued_process->next_process;
				}
			}
		}
	} else if(type == BLOCKED_MEM_STATE){
		if(bmem_queue[control_block->process_priority]->process_ID == control_block->process_ID){
			if(control_block->next_process != NULL){
				bmem_queue[control_block->process_priority] = control_block->next_process;
			} else {
				bmem_queue[control_block->process_priority] = NULL;
			}
		} else {
			queued_process = bmem_queue[control_block->process_priority];
			while(queued_process->next_process != NULL){
				if(queued_process->next_process == control_block){
					queued_process->next_process = control_block->next_process;
					break;
				} else {
					queued_process = queued_process->next_process;
				}
			}
		}
	}
	
	control_block->next_process = NULL;
	
	return control_block;
}

void context_switch(){	
	pcb * transition;
	pcb * temp;
	
	atomic_up();	
	transition = running;
	transition->frame_ptr = old_fp;
	
	if(transition->next_process != NULL){
		ready_queue[prio] = ready_queue[prio]->next_process;
		temp = ready_queue[prio];
		while(temp->next_process != NULL){
			temp = temp->next_process;
		}
		temp->next_process = transition;
		transition->next_process = NULL;
	}
	transition->process_state = READY_STATE;
	
	prio = 0;
	
	while(1){
		if(ready_queue[prio] != NULL){
			if(ready_queue[prio]->process_state == READY_STATE){
				running = ready_queue[prio];
				running->process_state = RUNNING_STATE;
				old_fp = running->frame_ptr;
				atomic_down();
				return;
				
			} else if(ready_queue[prio]->process_state == NEW_STATE){
				running = ready_queue[prio];
				running->process_state = RUNNING_STATE;
				atomic_down();
				temp_sp = running->stack_ptr;
				asm("move.l temp_sp, %d0");
				asm("move.l %d0, %a7");
				process_bootstrap(running);
				return;
			}
			break;			
		} else {
			prio = (prio + 1) % 4;
		}
	}
	return;
}

pcb * find_blocked_proc_by_id(int process_ID){
	pcb * temp;
	int i;
	for(i = 0; i < 4; i++){
		temp = bmsg_queue[i];
		while(temp != NULL){
			if(temp->process_ID == process_ID){
				return temp;
			} else {
				temp = temp->next_process;
			}
		}
	}
	return 0x00;
}

void atomic_up(){
	asm("move.w #0x2700, %sr");
	return;
}

void atomic_down(){
	asm("move.w #0x2000, %sr");	
	return;
}