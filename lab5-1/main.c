/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

/* -----------------------------------------------------------------
/* ---------- getmem test case ------------------------------------*/
uint32 flag_getmemevent_received;
uint32 flag_getmemtest_success;

void getmemevent_handler(uint32 event)
{
        if (event == PROCESS_GETMEM_EVENT)
        {
                flag_getmemevent_received = 1;
                kprintf("Expected event received: %d\n", event);
        }
        else
        {
                flag_getmemtest_success = 0;
                kprintf("Received an unexpected event: %d \n",event);
        }
}

void getmem_mytest()
{
        // initialize
        flag_getmemtest_success = 1;
        kprintf("Start of getmem test: \n");
        
        uint32 result = regevent(getmemevent_handler, PROCESS_GETMEM_EVENT);
        // check whether regist success
        if (result != OK)
        {
                kprintf("regevent failed: %d\n", result);
                return;
        }
        
        char* ptr = getmem(64);
        freemem(ptr, 64);
	sleep(2);
	if (flag_getmemevent_received != 1)
	{
	        flag_getmemtest_success = 0;
                kprintf("Failed to receive event\n");
	}
	
	// show test result
	if (flag_selfsendtest_success == 0)
                kprintf("Failed getmem test case!!\n");
        else
                kprintf("Pass getmem test case!!\n");
                
        kprintf("\n\n");
}


/* -------------------------------------------------------------
/* ----------------------- self send test case ----------------*/
uint32 selfsend_event;
uint32 flag_event_received;
uint32 flag_selfsendtest_success;

void selfsend_handler(uint32 event)
{
        if (event == selfsend_event)
        {
                flag_event_received = 1;
                kprintf("Expected event received: %d \n",event);
        }
        else
        {
                flag_event_received = 0;
                kprintf("Received an unexpected event: %d \n",event);
                flag_selfsendtest_success = 0;
        }
}

void selfsend_mytest()
{
        // initialize
        selfsend_event = 14;
        flag_event_received = 0;
        flag_selfsendtest_success = 1;
        kprintf("Start of self send test: \n");
        kprintf("The event index I set here: %d\n", selfsend_event);
        
        uint32 result = regevent(selfsend_handler, selfsend_event);
        // check whether regist success
        if (result != OK)
        {
                kprintf("regevent failed: %d\n", result);
                return;
        }

        result =  sendevent(getpid(), selfsend_event);       
        // check whether sended right
        if (result != OK)
        {
                kprintf("sendevent failed: %d\n", result);
                return;
        }
        sleep(2);
        // show test result
        if (flag_event_received == 0)
        {
                flag_selfsendtest_success = 0;
                kprintf("Failed to receive event %d\n", selfsend_event);
        }
        if (flag_selfsendtest_success == 0)
                kprintf("Failed self send test case!!\n");
        else
                kprintf("Pass self send test case!!\n");
                
        kprintf("\n\n");
}

/* ------------------------------------------------------
/* ---------- other process send test case ------------*/
uint32 otherproc_send_event;
uint32 flag_otherproc_event_received;
uint32 flag_otherproc_selfsendtest_success;

void otherproc_send_handler(uint32 event)
{
        if (event == otherproc_send_event)
        {
                flag_otherproc_event_received = 1;
                kprintf("Expected event received: %d \n",event);
        }
        else
        {
                flag_otherproc_event_received = 0;
                kprintf("Received an unexpected event: %d \n",event);
                flag_otherproc_selfsendtest_success = 0;
        }
}
void otherProc_send(pid32 pid, uint32 event_tosend)
{
        uint32 result = sendevent(pid, event_tosend);
        if (result != OK)
        {
                flag_otherproc_selfsendtest_success = 0;
                kprintf("Send fail\n");
        }
}
void otherproc_send_mytest()
{
        // initialize
        otherproc_send_event = 33;
        flag_otherproc_event_received = 0;
        flag_otherproc_selfsendtest_success = 1;
        kprintf("Start of self send test: \n");
        kprintf("The event index I set here: %d\n", otherproc_send_event);
        
        uint32 result = regevent(otherproc_send_handler, otherproc_send_event);
        // check whether regist success
        if (result != OK)
        {
                kprintf("regevent failed: %d\n", result);
                return;
        }
        
        pid32 sendProc_pid = create(process_send_process, 4096, 30, "Send Event", 2, getpid(), otherproc_send_event);
        resume(sendProc_pid);
        sleep(2);
        
        if(!isbadpid(sendProc_pid)) {
		kprintf("ERROR Send process still running\n");
		kill(sendProc_pid);
		flag_otherproc_selfsendtest_success = 0;
	}
	// show test result
	if (flag_otherproc_selfsendtest_success == 0)
                kprintf("Failed other process send test case!!\n");
        else
                kprintf("Pass other process send test case!!\n");
                
        kprintf("\n\n");
}

process	main(void)
{
	/* Put your test cases here */
	getmem_mytest();
	selfsend_mytest();
	otherproc_send_mytest();
	return OK;
}
