	.globl process_switch
	.even		
process_switch:
	move.w #0x2700, %sr
	move.l %d0, -(%a7)
	move.l %d1, -(%a7)
	move.l %d2, -(%a7)
	move.l %d3, -(%a7)
	move.l %d4, -(%a7)
	move.l %d5, -(%a7)
	move.l %d6, -(%a7)
	move.l %d7, -(%a7)
	move.l %a0, -(%a7)
	move.l %a1, -(%a7)
	move.l %a2, -(%a7)
	move.l %a3, -(%a7)
	move.l %a4, -(%a7)
	move.l %a5, -(%a7)
	move.l %fp, %d0
	move.l %d0, old_fp
		
	jsr	context_switch
	
	move.l old_fp, %d0
	move.l %d0, %a6
	move.l (%a7)+, %a5
	move.l (%a7)+, %a4
	move.l (%a7)+, %a3
	move.l (%a7)+, %a2
	move.l (%a7)+, %a1
	move.l (%a7)+, %a0
	move.l (%a7)+, %d7
	move.l (%a7)+, %d6
	move.l (%a7)+, %d5
	move.l (%a7)+, %d4
	move.l (%a7)+, %d3
	move.l (%a7)+, %d2
	move.l (%a7)+, %d1
	move.l (%a7)+, %d0
	move.w #0x2000, %sr
	
	rte
	
