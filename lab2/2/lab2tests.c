/* lab2tests.c - start_tests */

#include <xinu.h>



/*------------------------------------------------------------------------
 * start_tests  -  Lab 2 implementation
 *------------------------------------------------------------------------
 */
int32 global;
sid32 general;
sid32 sem;
int32 lost;
uint32 mutex;
pid32 pid[10];
int32 count[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
typedef void (*func) (int);
void disable_restore(int);
void semaphore(int);
void spin_lock(int);
void extra_credit2(int,int);	
void extra_credit1(int delaytime); 
func func_array[5] = {disable_restore, semaphore, spin_lock, extra_credit1, extra_credit2};
char* func_name[5] = {"disable_restore", "semaphore", "spin_lock", "extra_credit1", "extra_credit2"};


void	start_tests ()
{
	/* Your implementation of Lab 2 goes here */
	int32 i; 
	int32 j;
	int32 k;
	int32 rate;
	general = semcreate(0);
	sem = semcreate(1);	
	for (k = 100; k <= 400; k += 100){
		for (j = 0; j < 5; j++){
			global = 0;
			semdelete(general);
			general = semcreate(0);
			if (j == 1){
				semdelete(sem);
				sem = semcreate(1);
			}
			if (j == 2){
				mutex_unlock(&mutex);
			}
			if (j == 4) {
				for (i = 0; i < 10; i++){
					count[i] = 0;
				}
			}
			for (i = 0; i < 10; i++){
				if(j != 4){
					pid[i] = create(func_array[j], 4096, 20, func_name[j], 1, k);
					resume(pid[i]);
				}
				else {
					pid[i] = create(func_array[j], 4096, 20, func_name[j], 2, k, i);
					resume(pid[i]);
				}
			}
			uint32 start_time = getextime();
			uint32 old_time1 = clktime;
			signaln(general,10);
			sleep(30);
			for(i = 0; i < 10; i++){
				kill(pid[i]);
			}
			uint32 stop_time = getextime();
			if (j == 3){
				uint32 old_time2 = clktime;
				lost = 1000*((stop_time - start_time) - (old_time2 - old_time1)) / (stop_time - start_time);
			}
			if (j == 4){
				for (i = 0; i < 10; i++){
					global += count[i];
				} 
			}
			rate = global / (stop_time - start_time);
			if( j <= 2){		
				kprintf("Exclusive access method %s: delay of %d usecs, iter_rate %d \n", func_name[j], k, rate);
			}
			else if( j == 3){ 
				kprintf("Exclusive access method %s: delay of %d usecs, %d milli-seconds lost per second\n", func_name[j], k, lost);
			}
			else if( j == 4){		
				kprintf("Non-exclusive access method %s: delay of %d usecs, iter_rate %d \n", func_name[j], k, rate);
			}
		}
	}
}

void disable_restore(int delaytime){
	wait(general);
	while(TRUE){
		intmask mask = disable();
		global++;
		delayus(delaytime);
		restore(mask);
	}
}
	
void semaphore(int delaytime){
	wait(general);
	while(TRUE){
		wait(sem);
		global++;
		delayus(delaytime);
		signal(sem);
	}
}
		
	
void spin_lock(int delaytime){
	wait(general);
	while(TRUE){
		mutex_lock(&mutex);
		global++;
		delayus(delaytime);
		mutex_unlock(&mutex);
	}
}

void extra_credit1(int delaytime){
	wait(general);
	while(TRUE){
		intmask mask = disable();
		delayus(delaytime);
		restore(mask);
	}
} 
		
void extra_credit2(int delaytime, int i){ 
	wait(general);
	while(TRUE){
		count[i]++;
		delayus(delaytime);
	}
}

	


			
					
	

