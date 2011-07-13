#include "../debug/dbug.h"
#include "../shared/define.h"
#include "init.h"
#include "../memory/memory.h"
#include "../process_management/process_management.h"
#include "../io/uart.h"
#include "../rtx/rtx_inc.h"

pib * create_pib(UINT32 process_ID, UINT32 process_priority, UINT32 stack, void * process, void * next_process)
{
	pib * initialize_pib = (pib *)malloc(sizeof(pib));
	initialize_pib->process_ID = process_ID;
	initialize_pib->process_priority = process_priority;
	initialize_pib->process = process;
	initialize_pib->stack = stack;
	initialize_pib->next_process = next_process; 
	initialize_pib->control_block = (pcb *)malloc(sizeof(pcb)); 
	return initialize_pib;
}

void init_traps(){
	asm("MOVE.L #process_switch,%d0");
	asm("MOVE.L %d0,0x10000080"); //TRAP #0
	
	asm("MOVE.L #display_task_manager,%d0");
	asm("MOVE.L %d0,0x1000008C"); //TRAP #3
	
	asm("MOVE.L #display_all,%d0");
	asm("MOVE.L %d0,0x10000090"); //TRAP #4
	return;
}

void init_memory()
{
	int i;
	for(i=0; i < 32; i++){
		memoryBlocks[i].status = 0;
		memoryBlocks[i].location = malloc(128);
	}
	return;
}

void init_mailbox()
{
	red_lever = 0;

	int i;
	for(i=0; i < 32; i++){
		mailbox[i].sender_ID = -1;
		mailbox[i].destination_ID = -1;
		mailbox[i].msg_type = -1;
		mailbox[i].delay = -1;
	}
	return;
}

void init_queues(){
	clear_priority();
	int priority;
	
	for( priority = 0; priority < 4; priority++ ){
		ready_queue[priority] = NULL;
		bmsg_queue[priority] = NULL;
		bmem_queue[priority] = NULL;
	}
	return;
}

void init_scheduler(){
	total_procs = NUM_PROCESSES;
	prio = 0;

	pib * enqueue_process = init_blocks_head->next_process;
	while( enqueue_process != NULL ){		
		enqueue_process->control_block = insert_new_process(enqueue_process);
		enqueue_process = enqueue_process->next_process;
	}
	return;
}

void init_io(){
	UINT32 mask;
    atomic_up();
    asm( "move.l #asm_serial_entry,%d0" );
    asm( "move.l %d0,0x10000100" );
    SERIAL1_UCR = 0x10;
    SERIAL1_UCR = 0x20;
    SERIAL1_UCR = 0x30;
    SERIAL1_UCR = 0x40;
    SERIAL1_ICR = 0x17;
    SERIAL1_IVR = 64;
    SERIAL1_IMR = 0x02;
    SERIAL1_UBG1 = 0x00;
	#ifdef _CFSERVER_       /* add -D_CFSERVER_ for cf-server build */
    SERIAL1_UBG2 = 0x49;    /* cf-server baud rate 19200 */ 
	#else
    SERIAL1_UBG2 = 0x92;    /* regular baud rate 9600 */
	#endif 
    SERIAL1_UCSR = 0xDD;
    SERIAL1_UMR = 0x13;
    SERIAL1_UMR = 0x07;
    SERIAL1_UCR = 0x05;
    mask = SIM_IMR;
    mask &= 0x0003dfff;
    SIM_IMR = mask;
	atomic_down();
	SERIAL1_IMR = 3;
	return;
}

void init_timer(){
    atomic_up();
    asm( "move.l #asm_timer_entry,%d0" );
    asm( "move.l %d0,0x10000078" );
    TIMER0_ICR = 0x9B;
    TIMER0_TRR = 1758;
    TIMER0_TMR = 0xFF1B;
	timer_display = 0;
	atomic_down();
	return;
}

void register_commands(){
	kcd_msg = (CHAR *)malloc(64);
	msg_length = 0;
	current_command = OTHER;
	init_key_head = register_command((CHAR *)"W");	
	register_command((CHAR *)"WS");
	register_command((CHAR *)"WT");
	register_command((CHAR *)"C");
	register_command((CHAR *)"WS");
	
	return;
}

void system_kickoff(){
	running = init_blocks_head->control_block; 
	running->process_state = RUNNING_STATE;
	proc_pointer = (&null_process);
	return;
}

void null_process( void ){
	release_processor();
	return;
}

void sleep( int time ){
	int counter = 0;
	while( counter < (time*100000) ){
		counter++;
	}
	return;
}