/*  main.c  - main */

#include <xinu.h>

umsg32 dequeueue();
void enqueueue(pid32, umsg32);
syscall	mysend(pid32, umsg32);
uint32	mysendn(pid32, umsg32*,uint32);
umsg32	myreceive(void);
syscall	myreceiven(umsg32*,uint32);
umsg32	myrecvtime(int32,umsg32*,uint32);
sid32 general;
umsg32 message = 0;
umsg32 msends[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
umsg32* ptr = &msends[0];
uint32 msend = 9;
uint32 mreceive = 9;
umsg32* new_msgs;
umsg32* old_msgs;
pid32 pid[2]; 
int32 maxwait = 1000;


void cocosend1(pid32 pid, umsg32 msg){
		wait(general);
		mysend(pid, msg);		
}

void cocosend2(pid32 pid, umsg32* msgs, uint32 msend){
		wait(general);
		mysendn(pid, msgs, msend);
}
		
void cocoreceive1(){
		wait(general);		
		int mes = myreceive();
		kprintf("The message I received is %d\n", mes);
		kprintf("\n");
}

void cocoreceive2(umsg32* new_msgs, uint32 mreceive){
		wait(general);
		int i;
		myreceiven(new_msgs,mreceive);
		for(i = 0; i < mreceive; i++){
			kprintf("The message I received is %d\n", *(new_msgs+i));
		}
		kprintf("\n");
}

void cocorecvtime( int32 maxwait, umsg32* msgs, uint32 msg_count){
		wait(general);
		int num;
		int i = 0;
		num = myrecvtime(maxwait, msgs, msg_count);
		kprintf("there are %d messages received. And these messages are:\n", num);
		for(; i < num; i++) {
			kprintf(" %d ", *(msgs + i));
		}
		kprintf("\n");
}


process	main(void)
{
	general = semcreate(0);
	int i;
	pid[0] = create(cocoreceive1, 4096, 20, "cocoreceive1", 0);
	pid[1] = create(cocosend1, 4096, 20, "cocosend1", 2, pid[0], message);
	for (i = 0; i < 2; i++){
		struct procent *prptr = &proctab[pid[i]];
		prptr->cap = 20;
		prptr->head = 0;
		prptr->tail = 0;
		prptr->count = 0;
	}
	kprintf("Test of function mysend and myreceive:\n");
	resume(pid[0]);
	resume(pid[1]);
	signaln(general,2);
	sleep(2);
	kill(pid[0]);
	kill(pid[1]);
	semdelete(general);


	general = semcreate(0);
	pid[0] = create(cocoreceive2, 4096, 20, "cocoreceive2", 2, new_msgs, mreceive);
	pid[1] = create(cocosend2, 4096, 20, "cocosend2", 3, pid[0], ptr, msend);
	for (i = 0; i < 2; i++){
		struct procent *prptr = &proctab[pid[i]];
		prptr->cap = 20;
		prptr->head = 0;
		prptr->tail = 0;
		prptr->count = 0;
	}
	kprintf("Test of function mysendn and myreceiven:\n");
	resume(pid[0]);
	resume(pid[1]);
	signaln(general,2);
	sleep(2);
	kill(pid[0]);
	kill(pid[1]);
	semdelete(general);


	general = semcreate(0);
	pid[0] = create(cocorecvtime, 4096, 10, "cocorecvtime", 3, maxwait, old_msgs, 9);
	pid[1] = create(cocosend2, 4096, 10, "cocosend21", 3, pid[0], ptr, msend);
	for (i = 0; i < 2; i++){
		struct procent *prptr = &proctab[pid[i]];
		prptr->cap = 20;
		prptr->head = 0;
		prptr->tail = 0;
		prptr->count = 0;
	}
	kprintf("Test of function myrecvtime:\n");
	resume(pid[0]);
	resume(pid[1]);
	signaln(general,2);
	sleep(3);
	kill(pid[0]);
	kill(pid[1]);
	semdelete(general);

	return OK;
}
