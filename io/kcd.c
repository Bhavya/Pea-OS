#include "../rtx/rtx_inc.h"
#include "../debug/dbug.h"
#include "../shared/define.h"
#include "uart.h"
#include "../messaging/messaging.h"

void kcd(){
	while(1){
		void * msg = receive_message(NULL);
		CHAR * s = read_message(msg);
		rtx_dbug_outs(s);
		rtx_dbug_outs((CHAR *)"OHHH NOO");
	}
	return;
}