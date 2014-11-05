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
        intmask mask;
    
        mask = disable();
        
        // corner case: badpid
        if (isbadpid(pid))
        {
                restore(mask);
                return SYSERR;
        }
        
	// 1.	// if this event not have an event handler registered
	if (event_table[pid - 1][event-1] == 0)
	{
	        restore(mask);
	        return SYSERR;
	}

	// 2.   // if sender trying to send an OS reserved event
		      // or some other invalid event(10 ~ 50 is valid)
        if (event<USER_EVENT_BOUNDARY || event>MAX_EVENTS)
        {
                restore(mask);
                return SYSERR;
        }
        
    global_pid = pid;
  
	// send the event to the target process
        send(eventprocesspid, event);
        
        restore(mask);
	return OK;
}

/*sendevent for operating system*/
syscall sendeventSystem(pid32 pid, uint32 event){
        intmask mask;
    
        mask = disable();
        
        // corner case: badpid
        if (isbadpid(pid))
        {
                restore(mask);
                return SYSERR;
        }
        
	// 1.	// if this event not have an event handler registered
	if (event_table[pid - 1][event-1] == 0)
	{
	        restore(mask);
	        return SYSERR;
	}

	// 2.   // if invalid event
        if (event>=USER_EVENT_BOUNDARY || event<=0)
        {
                restore(mask);
                return SYSERR;
        }
        
   
        global_pid = pid;
	// send the event to the target process
        send(eventprocesspid, event);
        
        restore(mask);
	return OK;
}
