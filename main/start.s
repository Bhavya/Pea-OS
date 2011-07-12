

	/* Allocate space for new stack and old stack pointer */
	.comm old_stack,4
	.comm main_stack,4096

	.even		

	/* Install a new stack */
	move.l %a7,old_stack
	move.l #main_stack+4096, %a7

	/* Jump into main */
	jsr main
	
	/* Restore old stack */
	move.l old_stack,%a7

	/* Store return value from main */
	move.l %d0, %d7
	
	/* Get back to the monitor */
	move.l #0,%d0 
	trap #15
