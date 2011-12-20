/*--------------------------------------------------------------------------
 *                      RTX Test Suite 
 *--------------------------------------------------------------------------
 */
/**
 * @file:   rtx_test_dummy.c   
 * @author: Bhavya Kashyap
 * @date:   2011.07.11
 * @brief:  rtx test suite 
 */

 /*please fix as necessary*/
#include "../rtx/rtx_test.h"
#include "../debug/dbug.h"
#include "../shared/define.h"

UINT32 RTX_SUCCESS_COUNT;
UINT32 RTX_FAIL_COUNT;
int return_value;
void * received;
void * mblocks[32];

void success_rate(int success, int failure){
	if(failure == 0){
		rtx_dbug_outs((CHAR *)"\r\nOVERALL: PASS\r\n");
	} else {
		rtx_dbug_outs((CHAR *)"\r\nOVERALL: FAIL\r\n");
	}
	RTX_SUCCESS_COUNT += success;
	RTX_FAIL_COUNT += failure;
	return;
}

/* third party dummy test process 1 */ 
void test1()
{
	rtx_dbug_outs((CHAR *)"\r\nG021_RTX TEST START");
	RTX_SUCCESS_COUNT = 0;
	RTX_FAIL_COUNT = 0;
	int success = 0;
	int failure = 0;
	int i;
	
	rtx_dbug_outs((CHAR *)"\r\nTest1 Round #1");
	void * test1proc = g_test_fixture.request_memory_block();
	if(test1proc != 0x00){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: fails to request memory block.");
	}
	
	//release the memory location twice
	return_value = g_test_fixture.release_memory_block(test1proc);
	if(return_value >= 0){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: fails to release memory block.");
	}
	return_value = g_test_fixture.release_memory_block(test1proc);
	if(return_value != 0){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: releases the same memory block twice.");
	}	
	
	success_rate(success, failure);
	g_test_fixture.release_processor();
	
	rtx_dbug_outs((CHAR *)"\r\nTest1 Round #2");
	success = 0;
	failure = 0;
	
	for(i=0; i<32; i++){
		received = g_test_fixture.receive_message(NULL);
		if(received != 0x00){
			if(*((CHAR*)received+64) =='W'){
				success++;
				rtx_dbug_outs((CHAR *)"\r\nPASS");
			} else {
				failure++;
				rtx_dbug_outs((CHAR *)"\r\nFAIL: message envelope contains incorrect content.");
			}
		} else {
			failure++;
			rtx_dbug_outs((CHAR *)"\r\nFAIL: no message received.");
		}
	}
	success_rate(success,failure);
	g_test_fixture.release_processor();
	return;
}

/* third party dummy test process 2 */ 
void test2()
{
	rtx_dbug_outs((CHAR *)"\r\nTest2 Round #1");
	int success = 0;
	int failure = 0;
	int i;
	
	return_value = g_test_fixture.set_process_priority(2,4); 
	if(return_value < 0){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: tries to set process priority to hidden priority.");
	}
	
	success_rate(success, failure);	
	g_test_fixture.release_processor();	
	
	rtx_dbug_outs((CHAR *)"\r\nTest2 Round #2");
	success = 0;
	failure = 0;
	
	for(i=0; i<32; i++){
		return_value = g_test_fixture.release_memory_block(mblocks[i]);
		if(return_value >= 0){
			success++;
			rtx_dbug_outs((CHAR *)"\r\nPASS");
		} else {
			failure++;
			rtx_dbug_outs((CHAR *)"\r\nFAIL: fails to release memory block.");
		}
	}
	
	received = g_test_fixture.receive_message(NULL);
	rtx_dbug_outs((CHAR *)"\r\n\r\nTest2 Round #3");
	
	if(received != 0x00){
		if(*((CHAR*)received+64) =='D'){
			success++;
			rtx_dbug_outs((CHAR *)"\r\nPASS");
		} else {
			failure++;
			rtx_dbug_outs((CHAR *)"\r\nFAIL: message envelope contains incorrect content.");
		}
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: no message received.");
	}
	
	rtx_dbug_outs((CHAR *)"\r\nTESTS COMPLETE.");
	success_rate(RTX_SUCCESS_COUNT, RTX_FAIL_COUNT);
	asm("move.l #0,%d0");
	asm("TRAP #15");
	
	
	return;
}
/* third party dummy test process 3 */ 
void test3()
{
	rtx_dbug_outs((CHAR *)"\r\nTest3 Round #1");
	int success = 0;
	int failure = 0;
    return_value = g_test_fixture.set_process_priority(6,-1); 
	if(return_value < 0){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: tries to set invalid priority.");
	}
	
	return_value = g_test_fixture.set_process_priority(-1,2);  
	if(return_value < 0){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: tries to change priority of invalid process ID.");
	}
		
	// PID 0 returns 4
	return_value = g_test_fixture.get_process_priority(0);
	if(return_value == 4 ){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: get_process_priority returns incorrent priority for process 0.");
	}
	
	// invalid PID returns -1
	return_value = g_test_fixture.get_process_priority(999);
	if(return_value < 0 ){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: get_process_priority returns priority for invalid process.");
	}
	
	return_value = g_test_fixture.set_process_priority(0,2);  
	if(return_value < 0){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: tries to change priority of null process.");
	}
	
	success_rate(success, failure);
  	g_test_fixture.release_processor();
	
	rtx_dbug_outs((CHAR *)"\r\n\r\nTest3 Round #2");
	success = 0;
	failure = 0;
	received = g_test_fixture.request_memory_block();
	if(received != 0x00){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: fails to request memory block.");
	}
	
	return_value = g_test_fixture.send_message(2, received);
	if(return_value >= 0){
		*((CHAR*)received+64) = 'D';
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: fails to request memory block.");
	}
	
	g_test_fixture.release_processor();
	return;
}

/* third party dummy test process 4 */ 
void test4()
{
	rtx_dbug_outs((CHAR *)"\r\nTest4 Round #1");
	int success = 0;
	int failure = 0;
	int i;
	
  	return_value = g_test_fixture.set_process_priority(4,1);  
	if(return_value >= 0){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: fails to change own priority.");
	}
	
	for(i=0; i<32; i++){
		mblocks[i] = g_test_fixture.request_memory_block();
		if(mblocks[i] != 0x00){
			success++;
			rtx_dbug_outs((CHAR *)"\r\nPASS");
		} else {
			failure++;
			rtx_dbug_outs((CHAR *)"\r\nFAIL: fails to request memory block.");
		}
	}
	
	for(i=0; i<32; i++){
		return_value = g_test_fixture.release_memory_block(mblocks[i]);
		if(return_value >= 0){
			success++;
			rtx_dbug_outs((CHAR *)"\r\nPASS");
		} else {
			failure++;
			rtx_dbug_outs((CHAR *)"\r\nFAIL: fails to release memory block.");
		}
	}
	
	return_value = g_test_fixture.set_process_priority(4,3);  
	if(return_value >= 0){
		success++;
		rtx_dbug_outs((CHAR *)"\r\nPASS");
	} else {
		failure++;
		rtx_dbug_outs((CHAR *)"\r\nFAIL: fails to process own priority.");
	}
	
	success_rate(success, failure);	
	g_test_fixture.release_processor();
}
/* third party dummy test process 5 */ 
void test5()
{
	rtx_dbug_outs((CHAR *)"\r\nTest5 Round #1");
	int success = 0;
	int failure = 0;
	int i;
	
	for(i=0; i<32; i++){
		return_value = g_test_fixture.release_memory_block(mblocks[i]);
		if(return_value < 0){
			success++;
			rtx_dbug_outs((CHAR *)"\r\nPASS");
		} else {
			failure++;
			rtx_dbug_outs((CHAR *)"\r\nFAIL: tries to release memory block that has already been released.");
		}
	}
	
	success_rate(success, failure);	
  	g_test_fixture.release_processor();
	return;
}
/* third party dummy test process 6 */ 
void test6()
{
	rtx_dbug_outs((CHAR *)"\r\nTest6 Round #1");
	int success = 0;
	int failure = 0;
	int i;
	
	for(i=0; i<32; i++){
		received = g_test_fixture.request_memory_block();
		if(received != 0x00){
			*((CHAR*)received+64) = 'W';
			success++;
			rtx_dbug_outs((CHAR *)"\r\nPASS");
		} else {
			failure++;
			rtx_dbug_outs((CHAR *)"\r\nFAIL: fails to request memory block.");
		}
	}
	
	for(i=0; i<32; i++){
		return_value = g_test_fixture.send_message(1,mblocks[i]);
		if(return_value >= 0){
			success++;
			rtx_dbug_outs((CHAR *)"\r\nPASS");
		} else {
			failure++;
			rtx_dbug_outs((CHAR *)"\r\nFAIL: fails to send message.");
		}
	}
	
	success_rate(success, failure);
  	g_test_fixture.release_processor();
	return;
}

/* register the third party test processes with RTX */
void __attribute__ ((section ("__REGISTER_TEST_PROCS__")))register_test_proc()
{
    rtx_dbug_outs((CHAR *)"rtx_test: register_test_proc()\r\n");
	int i;
	
    for (i =0; i< NUM_TEST_PROCS; i++ ) {
        g_test_proc[i].pid = i + 1;
        g_test_proc[i].priority = 3;
        g_test_proc[i].sz_stack = 64;
    }
    g_test_proc[0].entry = &test1;
    g_test_proc[1].entry = &test2;
    g_test_proc[2].entry = &test3;
    g_test_proc[3].entry = &test4;
    g_test_proc[4].entry = &test5;
    g_test_proc[5].entry = &test6;
	return;
}

/**
 * Main entry point for this program.
 * never get invoked
 */
int main(void)
{
    rtx_dbug_outs((CHAR *)"rtx_test: started\r\n");
    return 0;
}
