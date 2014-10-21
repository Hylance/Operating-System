/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>


/*------------------------------------------------------------------------
 *  Self send test case - a process registers an event handler and sends
 *     an event to itself
 *------------------------------------------------------------------------
 */
uint32 self_event;
process	main(void)
{
	test_selfsend();
	test_getfreemem();
	test_other_send();
	return OK;
}
void self_handler(uint32 event) 
{
	if(event == self_event) {
		kprintf("Received expected event: %d\n", event);
		kprintf("success\n");
	} else {
		kprintf("Received unexpected event: %d\n", event);
		kprintf("failed\n");
	}
}
void test_selfsend(void)
{
	self_event = 20;
	kprintf("Self send testcase\n");
	kprintf("The message I want to send is %d\n", self_event);
	regevent(self_handler, self_event);
	sendevent(getpid(), self_event);
	printf("\n\n");
}

/*------------------------------------------------------------------------
 *  getmem  and freemem test case - a process registers an event handler for the getmem
 *     OS event and then allocates memory
 *------------------------------------------------------------------------
 */
void getmem_event_handler(uint32 event)
{
	if(event == PROCESS_GETMEM_EVENT) {
		kprintf("Received expected event: %d\n", event);
		kprintf("success\n");
	} else {
		kprintf("Received unexpected event: %d\n", event);
		kprintf("failed\n");
	}
}
void freemem_event_handler(uint32 event)
{
    if(event == PROCESS_FREEMEM_EVENT) {
        kprintf("Received expected event: %d\n", event);
        kprintf("success\n");
    } else {
        kprintf("Received unexpected event: %d\n", event);
        kprintf("failed\n");
    }
}

void test_getfreemem(void)
{
	char* ptr;
	kprintf("getmem and freemem testcase\n");
	regevent(getmem_event_handler, PROCESS_GETMEM_EVENT);
    regevent(freemem_event_handler, PROCESS_FREEMEM_EVENT);
	kprintf("The message I want to send is %d and %d \n", PROCESS_GETMEM_EVENT, PROCESS_FREEMEM_EVENT);
	ptr = getmem(32);
	freemem(ptr, 32);
	sleep(2);
	printf("\n\n");
}



/*------------------------------------------------------------------------
 *  Other process send test case - a process registers for an event
 *     then starts another process to send it the event.
 *------------------------------------------------------------------------
 */
uint32 process_expected_event;
void proccess_send_event_handler(uint32 event)
{
	if(event == process_expected_event) {
		kprintf("Received expected event: %d\n", event);
		kprintf("success\n");
	} else {
		kprintf("Received unexpected event: %d\n", event);
		kprintf("failed\n");
	}
}
void process_send_process(pid32 pid, uint32 event_to_send)
{
	sendevent(pid, event_to_send);
}
void test_other_send(void)
{
	pid32 send_process_pid;
	process_expected_event = 22;
	kprintf("Other process send testcase\n");
	regevent(proccess_send_event_handler, process_expected_event);
	kprintf("The message I want to send is %d\n", process_expected_event);
	send_process_pid = create(process_send_process, 4096, 30, "Send Event", 2, getpid(), process_expected_event);
	resume(send_process_pid);
	sleep(2);
	if(!isbadpid(send_process_pid)) {
		kprintf("ERROR Send process still running\n");
		kill(send_process_pid);
	}
	printf("\n\n");
}


