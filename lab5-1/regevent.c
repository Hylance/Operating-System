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
	intmask mask;
	mask = disable();


	// corner case
	if (event<=0 || event>MAX_EVENTS)
	{
	        restore(mask);
	        return SYSERR;       
	}
	
	// store function pointer if event happens	
	// additional calls, rewrite the exiting event handler
    event_table[getpid() - 1][event - 1] = eventhandler;  // event table
	
	restore(mask);
	return OK;
}
