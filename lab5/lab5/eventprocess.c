/* eventprocess.c - eventprocess */

#include <xinu.h>

pid32 eventprocesspid;

/*------------------------------------------------------------------------
 *  eventprocess  -  Process for calling event handlers
 *------------------------------------------------------------------------
 */
void	eventprocess(void)
{
	umsg32 rcvd_msg;
	void (*eventhandler)(umsg32 event);
	
	while(1) {
		rcvd_msg = receive();
		uint32 event = (rcvd_msg & 0xFFFF);
		rcvd_msg = (rcvd_msg >> 16);
		struct	procent *prptr;	
		prptr = &proctab[rcvd_msg];
		eventhandler = prptr->event_table[event - 1];
		eventhandler(event);
	}	
}
