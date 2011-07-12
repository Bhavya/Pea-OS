#include "../rtx/rtx_inc.h"
#include "../debug/dbug.h"
#include "../shared/define.h"
#include "../timer/timer.h"

SINT32 Ticks = +8639000;
SINT32 Hours = 0;
SINT32 Minutes = 0;
SINT32 Seconds = 0;

VOID c_timer_handler( VOID )
{
    if( Ticks < 8640000 )
	{
		Ticks++;
	} 
	else 
	{
		Ticks = 0;
	}
    TIMER0_TER = 2;
	return;
}

void wallclock(){


}

int set_system_time(int hh, int mm, int ss){

	return 0;
}