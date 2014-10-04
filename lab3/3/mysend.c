/* mysend.c - mysend and mysendn */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  mysend  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */


void enqueueue(pid32 pid, umsg32 item){
	struct procent *prptr = &proctab[pid];
	prptr->message[prptr->tail] = item;
	prptr->tail = (prptr->tail + 1) % 20;
	prptr->count++;
	prptr->cap--;
}

syscall	mysend(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
    intmask mask;
    struct procent *prptr;
    
    mask = disable();
    if (isbadpid(pid)) {
        restore(mask);
        return SYSERR;
    }
    
    prptr = &proctab[pid];
    if (prptr->prstate == PR_FREE || prptr->count >= 20) {
        restore(mask);
        return SYSERR;
    }
    
    
    enqueue(pid, msg);
		
    
    
    if (prptr->prstate == PR_MYRECV) {
        ready(pid);
    } else if (prptr->prstate == PR_MYRECTIM) {
        unsleep(pid);
        ready(pid);
    }
    restore(mask);
    return OK;

}

/*------------------------------------------------------------------------
 *  mysendn  -  Pass 'n' messages to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
uint32	mysendn(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32* 	msgs,		/* Message contents		*/
	  uint32 	msg_count	/* Number of messages		*/
	)
{
    int i;
    int result = msg_count;
    intmask mask;
    struct procent *prptr;
    
    mask = disable();
    if (isbadpid(pid)) {
        restore(mask);
        return SYSERR;
    }
    
    prptr = &proctab[pid];
    if (prptr->prstate == PR_FREE) {
        restore(mask);
        return SYSERR;
    }
    
    if (prptr->cap >= msg_count){
        for (i = 0; i < msg_count; i++) {
            enqueueue(pid, *(msgs+i));
        }
    } else if(prptr->cap >= 0 && prptr->cap < msg_count) {
        for (i = 0; i < prptr->cap; i++){
            enqueueue(pid, *(msgs+i));
        }
        result = prptr->cap;
    }
    
    
    if (prptr->prstate == PR_MYRECV) {
        ready(pid);
    } else if (prptr->prstate == PR_MYRECTIM) {
        unsleep(pid);
        ready(pid);
    }
    restore(mask);
    
    return result;


}
