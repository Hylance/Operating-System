/*  farm.c  - definition of farmer and vegetarian lab */

#include <xinu.h>
#include "farm.h"

/*------------------------------------------------------------------------
 *  start_farm  -  Initializes and starts the farm simulation
 *------------------------------------------------------------------------
 */
	sid32 general, full, none, mutex;
	
	int32 pid[NFARMERS + NVEGETARIANS]; // record process id
	int32 p = 0;
	
	int32 buy[NVEGETARIANS][NFARMERS];//record how many carrots were bought by each vagetarian
	char q[FIELDSIZE];
	int32 count = 0; //to count carrots in field
	int32 head = 0;
	int32 tail = 0;
	int32 waitinnone = 0;
	int32 waitinfull = 0;
	void enqueueue(char);
	void dequeueue(int32);
	void produce(int32);
	void consume(int32);
	

 void start_farm(void)
{
	general = semcreate(0);// used to start all the processes at the same time
	full = semcreate(0);//farmer wait in full when the field is full
	none = semcreate(0);//vegetarian wait in none when the carrots in field are less than the number of carrots the vegetarian need
	mutex = semcreate(1);//ensure the queue operation is mutual exclusive
	
	int32 i = 0; 
	for(; i < NFARMERS; i++, p++){
		pid[p]= create(produce, 4096, 20, farmer_tags[i], 1, i);
		resume(pid[p]);
	}
	int32 j = 0;
	for(; j < NVEGETARIANS; j++, p++){
		pid[p] = create(consume, 4096, 20, vegetarian_tags[j], 1, j);
		resume(pid[p]);		
	}
	signaln(general, NFARMERS+NVEGETARIANS);
}



	
	void enqueueue(char item){
		q[tail] = item;
		tail = (tail + 1) % FIELDSIZE;
		count++;
	}
	void dequeueue(int32 j){
		char item = q[head];
		head = (head + 1) % FIELDSIZE;
		count--;
		int32 i = 0;
		for(; i < NFARMERS; i++){
			if (item == farmer_tags[i]){
				 //record who sell the carrot
				buy[j][i]++;
			
			 
			}
		}
	}
		
		
	void produce(int32 i){
		wait(general);//to put process to wait for starting  at the same time
		while(TRUE){			
			if( count >= FIELDSIZE){
				waitinfull++;
				wait(full);
				
			}
			else {
				sleepms(farmer_grow_times[i]);
				wait(mutex);
				enqueueue(farmer_tags[i]);
				if(waitinnone > 0){
					signal(none);
					waitinnone--;
				}
				signal(mutex);
				
			}
		}
	}



	void consume(int32 j){
		wait(general);
		while(TRUE){
			wait(mutex);
			if(count <= vegetarian_hungers[j]){
				waitinnone++;
				signal(mutex);				
				wait(none);
				continue;	
			}
			else{

				int32 k = 0;		
				for(; k < vegetarian_hungers[j]; k++){
					dequeueue(j);
					if(waitinfull > 0){
												
						signal(full);
						waitinfull--;
					}
				}
				signal(mutex);
				sleepms(vegetarian_sleep_times[j]);	
	
			}
		}
	}


	
/*------------------------------------------------------------------------
 *  stop_farm  -  Stops the currently executing farm simulation
 *------------------------------------------------------------------------
 */
void stop_farm(void)
{
	int32 i = 0;
	for(; i < NFARMERS + NVEGETARIANS; i++){
		kill(pid[p]);
	}

	
}

/*------------------------------------------------------------------------
 *  print_farm_report  -  Prints the final report for the farm simulation
 *------------------------------------------------------------------------
 */
void print_farm_report(void)
{
	int32 i = 0;
	for(; i < NFARMERS; i++){	
		int sum = 0;
		int u = 0;
		for(; u < NVEGETARIANS; u++){
			sum = sum + buy[u][i];
		}
		kprintf("Farmer %c : sold %d carrots\n", farmer_tags[i], sum);
	}
	int32 j = 0;
	
	for(; j < NVEGETARIANS; j++){
		kprintf("vegetarian %c :", vegetarian_tags[j]);
		int32 k = 0;
		for(; k < NFARMERS; k++){
			kprintf(" %d carrots from farmer %c,", buy[j][k], farmer_tags[k]);
		}
		kprintf("\n");
	}
}
