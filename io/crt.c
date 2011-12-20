#include "../rtx/rtx_inc.h"
#include "../debug/dbug.h"
#include "../shared/define.h"
#include "uart.h"
#include "../messaging/messaging.h"
#include "../memory/memory.h"

void crt(){
	void * msg;
	CHAR * s;
	while(1){
		msg = receive_message(NULL);
		*s = *(read_message(msg));
		uprintf(s);
		release_memory_block(msg);
	}
	return;
}