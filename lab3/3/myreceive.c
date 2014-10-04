/* myreceive.c - myreceive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  myreceive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */

umsg32 dequeueue() {
	struct procent *prptr = &proctab[currpid];
	umsg32 item = prptr->message[prptr->head];
	prptr->head = (prptr->head + 1) % 20;
	prptr->count--;
	prptr->cap++;
	return item;
}


umsg32	myreceive(void)
{
    intmask	mask;
    struct	procent *prptr;
    umsg32	msg;
    
    mask = disable();
    prptr = &proctab[currpid];
  
    if (prptr->count <= 0) {
        prptr->prstate = PR_MYRECV;
        resched();
    }

    msg = dequeueue();
    restore(mask);
    return msg;

}


/*------------------------------------------------------------------------
 *  myreceiven  -  Wait for 'n' messages and return to the caller
 *------------------------------------------------------------------------
 */
syscall	myreceiven(
	  umsg32* 	msgs,		/* Message contents		*/
	  uint32 	msg_count	/* Number of messages		*/
	)
{
    intmask	mask;
    struct	procent *prptr;
	int i;
	int mes;
    
    mask = disable();
    prptr = &proctab[currpid];
   
    while (prptr->count < msg_count) {
        prptr->prstate = PR_MYRECV;
        resched();
    } 

	for (i = 0; i < msg_count; i++){
		mes = dequeueue();
		*(msgs + i) = mes;
	}
    restore(mask);
    return OK;

}

/*---------------------------------------------------------------------------------------------------------
 *  Myrecvtime  -  Wait specified time to receive  queueed message and return number of message received
 *---------------------------------------------------------------------------------------------------------
 */

umsg32	myrecvtime(
	  int32		maxwait,
	  umsg32*	msgs,
	  uint32	msg_count
        )
{
	intmask	mask;			
	struct	procent	*prptr;					

	if (maxwait < 0) {
		return SYSERR;
	}
	mask = disable();

	
	prptr = &proctab[currpid];
	if (prptr->count == 0) {	
		if (insertd(currpid,sleepq,maxwait) == SYSERR) {
			restore(mask);
			return SYSERR;
		}
		prptr->prstate = PR_MYRECTIM;
		resched();
	}

		
	int msg = 0;
	while(prptr->count > 0) {
		msgs[msg] = dequeueue();
		msg++;
		if( msg == msg_count) {
			break;
		}
	}		

	restore(mask);
	return msg;
}
