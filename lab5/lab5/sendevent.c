/* sendevent.c - sendevent */

#include <xinu.h>

extern pid32 eventprocesspid;

/*------------------------------------------------------------------------
 *  sendevent  -  Sends an event to the given process
 *------------------------------------------------------------------------
 */
syscall	sendevent(
	  pid32		pid,		/* ID of recipient process	*/
	  uint32	event		/* Event to send		*/
	)
{
	int32 msg = pid;
	intmask	mask;			
	struct	procent *prptr;		
	mask = disable();
	if (isbadpid(pid) || event < USER_EVENT_BOUNDARY || event > MAX_EVENTS ) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->event_table[event - 1] == 0) {
		restore(mask);
		return SYSERR;
	}
	msg = (msg << 16);
	msg = msg + event;
	send(eventprocesspid, msg);	
	restore(mask);		
	return OK;
}
syscall	OS_sendevent(
	  pid32		pid,		/* ID of recipient process	*/
	  uint32	event		/* Event to send		*/
	)
{
	int32 msg = pid;
	intmask	mask;			
	struct	procent *prptr;		
	mask = disable();
	if (event >= USER_EVENT_BOUNDARY || event <= 0) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->event_table[event - 1] == 0 ) {
		restore(mask);
		return SYSERR;
	}
	msg = (msg << 16);
	msg = msg + event;
	send(eventprocesspid, msg);	
	restore(mask);		
	return OK;
}
