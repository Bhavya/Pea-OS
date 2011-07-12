extern int send_message (int process_ID, void * MessageEnvelope);
extern void * receive_message(int * sender_ID);
extern int delayed_send(int process_ID, void * MessageEnvelope, int delay);
int message_type( int );
void unblock_waiting_procs( void );
void set_message_type( int, int );
void * write_message( void *, CHAR * );
CHAR * read_message( void * );