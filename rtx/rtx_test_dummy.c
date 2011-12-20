/*--------------------------------------------------------------------------
 *                      RTX Test Suite 
 *--------------------------------------------------------------------------
 */
/**
 * @file:   rtx_test_dummy.c   
 * @author: Bhavya Kashyap
 * @author: Kristof Dorgai
 * @date:   2011.07.11
 * @brief:  rtx test suite 
 */

 /*please fix as necessary*/
#include "../rtx/rtx_test.h"
#include "../debug/dbug.h"
#include "../shared/define.h"

/* third party dummy test process 1 */ 
void test1()
{
	while(1){
		rtx_dbug_outs((CHAR *)"\r\nTest 1");
		g_test_fixture.release_processor();
	}
}

/* third party dummy test process 2 */ 
void test2()
{
	while(1){
		rtx_dbug_outs((CHAR *)"\r\nTest 2");
		g_test_fixture.release_processor();
	}
	return;
}
/* third party dummy test process 3 */ 
void test3()
{
	while(1){
		rtx_dbug_outs((CHAR *)"\r\nTest 3");
		g_test_fixture.release_processor();
	}
	return;
}

/* third party dummy test process 4 */ 
void test4()
{
	while(1){
		rtx_dbug_outs((CHAR *)"\r\nTest 4");
		g_test_fixture.release_processor();
	}
	return;
}
/* third party dummy test process 5 */ 
void test5()
{
	while(1){
		rtx_dbug_outs((CHAR *)"\r\nTest 5");
		g_test_fixture.release_processor();
	}
	return;
}
/* third party dummy test process 6 */ 
void test6()
{
	while(1){
		rtx_dbug_outs((CHAR *)"\r\nTest 6");
		g_test_fixture.release_processor();
	}
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
