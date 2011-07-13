#include "../rtx/rtx_inc.h"
#include "../debug/dbug.h"
#include "priority.h"
#include "../shared/define.h"
#include "../process_management/process_management.h"

int set_process_priority (int process_ID, int priority)
{
	pib * init_block = init_blocks_head;
	pcb * control_block;
	if(priority < 0 || priority > 3){
		#ifdef _ERR_
			exception(INVALID_PRIORITY);
		#endif
	} else if(process_ID < 1 || process_ID > 6 ){
		#ifdef _ERR_
			exception(SET_PRIO_INVALID_PROCESS); 
		#endif
	} else {
		while(init_block != NULL){
			control_block = init_block->control_block;
			if(init_block->process_ID == process_ID){		
				if(control_block->process_priority == priority){
					#ifdef _ERR_
						exception(SET_PRIO_SAME_PRIORITY); 
					#endif
				} else if(control_block->process_state == TERMINATED_STATE){
					#ifdef _ERR_
						exception(REFERENCE_TERMINATED_PROCESS); 
					#endif
				} else {
					insert_process(remove_process(control_block),priority);	
					control_block->process_priority = priority;		
					return RTX_SUCCESS;
				}
			}
			init_block = init_block->next_process;
			if(prio > priority){
				prio = priority;
			}
		}
	}
	return RTX_ERROR;
}

int get_process_priority (int process_ID)
{
    int priority;
	pib * init_block = init_blocks_head;
	pcb * control_block;
	if( process_ID == 0 ){
		return 0x4;
	} else {
		while(init_block != NULL){
			if(init_block->process_ID == process_ID){
				control_block = init_block->control_block;
				priority = control_block->process_priority;
				return priority;
			}
		init_block = init_block->next_process;
		}
	}
	
	#ifdef _ERR_
		exception(NON_EXISTENT_PROCESS);
	#endif
	return RTX_ERROR;
}