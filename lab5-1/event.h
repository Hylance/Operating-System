/* event.h */

/* Operating system events						*/
#define PROCESS_GETMEM_EVENT	2	/* Event generated when a 	*/
					/*   process successfully 	*/
					/*   allocates memory with 	*/
					/*   getmem			*/
#define PROCESS_FREEMEM_EVENT	3	/* Event generated when a 	*/
					/*   process succesfully	*/
					/*   frees memory with freemem	*/

/* User events - 							*/
/*    callers of sendevent cannot use event values less than the	*/
/*    USER_EVENT_BOUNDARY						*/
#define USER_EVENT_BOUNDARY	10	/* User events begin here	*/


#define MAX_EVENTS		50	/* Maximum number of events	*/
typedef void (*eventhandler)(umsg32 event);
int global_pid;
eventhandler event_table[8][50];


