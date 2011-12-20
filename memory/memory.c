#include "../rtx/rtx_inc.h"
#include "../debug/dbug.h"
#include "../shared/define.h"
#include "memory.h"
#include "../process_management/process_management.h"

//Variables
extern void * _end; // from the linker script file
void * heap = &_end; // global pointer to the start of the memory pool
//End Variables

//Functions and Structures
void * malloc(UINT32 size)
{
	void * oldHeap = heap;
	heap += size;

	int i;
	for(i = 0; i < size; i++) {
		*(BYTE *)(oldHeap+i) = 0x00;
	}
	return oldHeap;
}

void * proc_malloc()
{
	UINT32 size = 512; //each process gets a stack of 255 bytes
	void * oldHeap = heap;
	heap += size;

	int i;
	for(i = 0; i < size; i++) {
		*(BYTE *)(oldHeap+i) = 0x00;
	}
	return heap;
}

void unblock_single_waiting_process(){
	pcb * temp;
	int i;
	for(i=0; i<4; i++){
		temp = bmem_queue[i];
		if(temp == NULL) continue;
		while(temp->next_process != NULL){
			temp = temp->next_process;
		} 
		insert_process(remove_blocked_process(temp,BLOCKED_MEM_STATE), temp->process_priority);
	}
	return;
}

void * request_memory_block() 
{
    int i;
	for( i = 0; i < 32; i++ ){
		if( memoryBlocks[i].status != 1 )
		{
			memoryBlocks[i].status = 1;
			return memoryBlocks[i].location;
		}
	}
	#ifdef _ERR_
		exception(OUT_OF_MEM);
	#endif
	insert_blocked_process(remove_process(running), running->process_priority, BLOCKED_MEM_STATE);
	release_processor();	
	return request_memory_block();
}

int release_memory_block(void * memory_block)
{
    int i;
	for( i = 0; i < 32; i++){
		if( memoryBlocks[i].location == memory_block && memoryBlocks[i].status == 1 )
		{
			memoryBlocks[i].status = 0;
			unblock_single_waiting_process();
			return RTX_SUCCESS;
		}
	}
	#ifdef _ERR_
		exception(CANNOT_RELEASE);
	#endif
	return RTX_ERROR;
}
