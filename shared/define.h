#define NUM_MEM_BLKS 32
#define NUM_PROCESSES 6

#define KCD_PID 7
#define CRT_PID 8
#define WALLCLOCK_PID 9

/*Kernel Structure*/
enum state
{
    NEW_STATE,
    READY_STATE,
    RUNNING_STATE,
	BLOCKED_MSG_STATE,
	BLOCKED_MEM_STATE,
	TERMINATED_STATE
};

typedef enum state state;

struct pcb
{
	UINT32 process_ID;
	UINT32 process_priority; 
	state process_state;
	UINT32 frame_ptr;
	UINT32 stack_ptr;
	void * process;
	void * next_process;
};

typedef struct pcb pcb;

struct pib
{
	UINT32 process_ID;
	UINT32 process_priority; 
	UINT32 stack;
	void * process;
	void * next_process;
	pcb * control_block;
};

typedef struct pib pib;


/*Memory*/
extern void * malloc( UINT32 );
extern void * proc_malloc();

struct memoryBlock
{
	int status; 		
	void * location; 	
};

typedef struct memoryBlock memoryBlock;

memoryBlock memoryBlocks[32];


/*Messaging*/
struct msgEnvelope
{
	SINT32 sender_ID;
	SINT32 destination_ID;
	SINT32 msg_type;
	SINT32 delay;
	CHAR * msg_body;
};

typedef struct msgEnvelope msgEnvelope;

msgEnvelope mailbox[32];
msgEnvelope delayed_mailbox[32];

int red_lever;
int * sender_ID;


/*Scheduling*/
int total_procs;
VOID (*proc_pointer)( VOID );
pib * init_blocks_head;

pcb * ready_queue[4];
pcb * bmsg_queue[4];
pcb * bmem_queue[4];

pcb * running;

int prio;
UINT32 old_fp;
UINT32 temp_sp;

extern void block_self( state );

/*Interaction and IO*/
enum command
{
	W,
	WS,
	WT,
	C,
	OTHER,
	INVALID
};

typedef enum command command;

struct command_key
{
	CHAR * keystroke; 		
	void * next_key; 	
};

typedef struct command_key command_key;

command_key * init_key_head;
command_key * init_key_tail;
command current_command;
UINT32 command_flag;
CHAR * kcd_msg;
int msg_length;

/*UART*/
volatile BYTE CharIn;
volatile BYTE CharOut;

/*Timer*/
UINT32 timer_display;

/*Interrupts*/
extern void atomic_up( void );
extern void atomic_down( void );
void sleep( int );

/*Debugging*/
VOID task_manager( void );
VOID display_queue( state );
VOID display_queue_all( void );
VOID display_mailbox();
VOID display_hotkeys();