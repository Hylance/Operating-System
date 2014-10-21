/* regevent.c - regevent */

#include <xinu.h>

extern pid32 eventprocesspid;

/*------------------------------------------------------------------------
 *  regevent  -  Register an event handler
 *------------------------------------------------------------------------
 */
syscall	regevent(
	  void (*eventhandler)(umsg32 event),	/* Pointer to event	*/
						/*   handler		*/
	  uint32 event			/* Event to register for	*/
	)
{
	intmask	mask;
	struct	procent *prptr;
	mask = disable();
	if (event <= 0 || event > MAX_EVENTS ) {
		restore(mask);
		return SYSERR;
	}
	prptr = &proctab[currpid];
	prptr->event_table[event - 1] = eventhandler;
	restore(mask);
	return OK;
}
