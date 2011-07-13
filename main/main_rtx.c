/*----------------------------------------------------------------------------
 *              A Dummy RTX for Testing 
 *----------------------------------------------------------------------------
 */
/**
 * @file: main_rtx.c
 * @author: Thomas Reidemeister 
 * @author: Bhavya Kashyap 
 * @date: 2010.02.18
 * @brief: Example dummy rtx for testing
 */

#include "../debug/dbug.h"
#include "../shared/define.h"
#include "../memory/memory.h"
#include "../messaging/messaging.h"
#include "../process_management/process_management.h"
#include "../priority/priority.h"
#include "../io/uart.h"
#include "../timer/timer.h"
#include "../rtx/rtx_test.h"
#include "../rtx/rtx_inc.h"
#include "../rtx/rtx.h"
#include "init.h"


extern void __REGISTER_TEST_PROCS_ENTRY__();

void create_processes(){
	int counter;
	pib * test_process;	
	init_blocks_head = create_pib(0, 4, 64,  &null_process, NULL );
	test_process = init_blocks_head;

	for(counter=0; counter < NUM_PROCESSES; counter++){
		test_process->next_process = create_pib(g_test_proc[counter].pid, g_test_proc[counter].priority, g_test_proc[counter].sz_stack,  g_test_proc[counter].entry, NULL );
		test_process = test_process->next_process;
	}
	

	test_process->next_process = create_pib(CRT_PID, 0, 255,  &crt, NULL );
	test_process = test_process->next_process;
	
	test_process->next_process = create_pib(KCD_PID, 0, 255,  &kcd, NULL );
	test_process = test_process->next_process;
	
	test_process->next_process = create_pib(WALLCLOCK_PID, 0, 255,  &wallclock, NULL );
	
	return;
}

int __main( void )
{
    return 0;
}

int main() 
{
	rtx_dbug_outs((CHAR *)"rtx: Entering main()\r\n");
	
	__REGISTER_TEST_PROCS_ENTRY__();

	coldfire_vbr_init();	
	init_traps();
	init_memory();
	init_mailbox();
	init_queues();
	create_processes();
	init_scheduler();
	init_io();
	init_timer();
	register_commands();
	
	system_kickoff();
	(*proc_pointer)();
	
	rtx_dbug_outs((CHAR *)"\r\nrtx: Exiting main()\r\n");
	return 0; //END 
}


/* register rtx primitives with test suite */

void  __attribute__ ((section ("__REGISTER_RTX__"))) register_rtx() 
{
    rtx_dbug_outs((CHAR *)"rtx: Entering register_rtx()\r\n");
    g_test_fixture.send_message = &send_message;
    g_test_fixture.receive_message = &receive_message;
    g_test_fixture.request_memory_block = &request_memory_block;
    g_test_fixture.release_memory_block = &release_memory_block;
    g_test_fixture.release_processor = &release_processor;
    g_test_fixture.delayed_send = &delayed_send;
    g_test_fixture.set_process_priority = &set_process_priority;
    g_test_fixture.get_process_priority = &get_process_priority;
    rtx_dbug_outs((CHAR *)"rtx: leaving register_rtx()\r\n");
	return;
}
