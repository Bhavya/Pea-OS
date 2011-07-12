extern int release_processor();
extern void context_switch( void );

extern int insert_process(pcb *, UINT32);
extern pcb * insert_new_process(pib *);
extern pcb * remove_process(pcb *);	
extern int insert_blocked_process(pcb *, UINT32, state);
extern pcb * remove_blocked_process(pcb *, state);
pcb * find_blocked_proc_by_id(int);