/*--------------------------------------------------------------------------
 *                      RTX Stress Test
 *--------------------------------------------------------------------------
 */
/**
 * @file:   rtx_test_dummy.c   
 * @author: Bhavya Kashyap
 * @author: Kristof Dorgai
 * @date:   2011.07.11
 * @brief:  rtx stress test suite 
 */

#include "../rtx/rtx_test.h"
#include "../debug/dbug.h"
#include "../shared/define.h"
#include "../io/uart.h"

#define NUM_TEST_PROCS 3
#define STRESS_A_PID 1
#define STRESS_A_PID 2
#define STRESS_C_PID 3

#define COUNT_REPORT 1
#define WAKEUP_10 2

int index;
struct node{
	void * a;
	void * n;
};
typedef node node;

void stress_a(){
	int num;
	void * p = request_memory_block();
	register_command((CHAR *)"Z");
	while(1){
		if(current_command->keystroke) == OTHER){
			release_memory_block(p);
			break;
		} else {
			release_memory_block(p);
		}
	}
	num = 0;
	while(1){
		p = request_memory_block();
		index = send_message(STRESS_A_PID, p);
		set_message_type(index, COUNT_REPORT);
		*((int *)(p+64)) = num;
		num++;
		release_processor();
	}
	return;
}

void stress_b(){
	void * p;
	while(1){
		p = receive_message(NULL);
		index = send_message(STRESS_C_PID, p);
		set_message_type(index, COUNT_REPORT);
	}	
	return;
}

void stress_c(){
	void * p;
	void * q;
	node * head = (node *)malloc(sizeof(node));
	node *tail;
	queue_size = 0;
	
	while(1){
		if(queue_size == 0){
			p = receive_message();
			head = enqueue(p);
		} else {
			p = dequeue();
			queue_size--;
		}
		if(message_type(index) == COUNT_REPORT){
			if(*((int*)(p+64))%20 == 0){
				uprintf((CHAR *)"Process C");
			}
			q = request_memory_block();
			index = delayed_send(STRESS_C_PID, q, 10);
			set_message_type(index, WAKEUP_10);
			while(1){
				p = receive_message(NULL);
				if(message_type(index) == WAKEUP_10){
					break;
				}
			} else {
				enqueue(p);
			}
		}	
		release_processor();		
	}	
	return;
}

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