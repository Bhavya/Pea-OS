/*--------------------------------------------------------------------------
 *                      RTX Stress Test
 *--------------------------------------------------------------------------
 */
/**
 * @file:   rtx_test_dummy.c   
 * @author: Bhavya Kashyap
 * @date:   2011.07.11
 * @brief:  rtx stress test suite 
 */

#include "../rtx/rtx_test.h"
#include "../debug/dbug.h"
#include "../shared/define.h"
#include "../io/uart.h"
#include "../messaging/messaging.h"

#define STRESS_A_PID 1
#define STRESS_B_PID 2
#define STRESS_C_PID 3

#define COUNT_REPORT 1
#define WAKEUP_10 2

int ml_index;
struct node{
	void * a;
	void * n;
};
typedef struct node node;
node * head;
node * tail;
queue_size = 0;

node * enqueue(void * a){
	tail->n = (node*)malloc(sizeof(node));
	tail = tail->n;
	tail->a = a;
	tail->n = NULL;
	return tail;
}

node * dequeue(){
	node * temp = head;
	head = head->n;
	return temp;
}

void stress_a(){
	int num;
	void * p = g_test_fixture.request_memory_block();
	register_command((CHAR *)"Z");
	while(1){
		if(current_command == OTHER){
			g_test_fixture.release_memory_block(p);
			break;
		} else {
			g_test_fixture.release_memory_block(p);
		}
	}
	num = 0;
	while(1){
		p = g_test_fixture.request_memory_block();
		ml_index = g_test_fixture.send_message(STRESS_A_PID, p);
		set_message_type(ml_index, COUNT_REPORT);
		*((int *)(p+64)) = num;
		num++;
		g_test_fixture.release_processor();
	}
	return;
}

void stress_b(){
	void * p;
	while(1){
		p = g_test_fixture.receive_message(NULL);
		ml_index = g_test_fixture.send_message(STRESS_C_PID, p);
		set_message_type(ml_index, COUNT_REPORT);
	}	
	return;
}

void stress_c(){
	void * p;
	void * q;
	head = (node *)malloc(sizeof(node));
	while(1){
		if(queue_size == 0){
			p = g_test_fixture.receive_message(NULL);
			head = enqueue(p);
		} else {
			p = dequeue();
			queue_size--;
		}
		if(message_type(ml_index) == COUNT_REPORT){
			if(*((int*)(p+64))%20 == 0){
				uprintf((CHAR *)"Process C");
			}
			q = g_test_fixture.request_memory_block();
			ml_index = g_test_fixture.delayed_send(STRESS_C_PID, q, 10);
			set_message_type(ml_index, WAKEUP_10);
			while(1){
				p = g_test_fixture.receive_message(NULL);
				if(message_type(ml_index) == WAKEUP_10){
					break;
				} else {
					enqueue(p);
				}
			} 
		}	
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
    g_test_proc[0].entry = &stress_a;
    g_test_proc[1].entry = &stress_b;
    g_test_proc[2].entry = &stress_c;
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