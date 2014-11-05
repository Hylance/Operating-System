/* eventprocess.c - eventprocess */

#include <xinu.h>

pid32 eventprocesspid;     // process identifier



/*------------------------------------------------------------------------
 *  eventprocess  -  Process for calling event handlers
 *------------------------------------------------------------------------
 */
void	eventprocess(void)
{
	umsg32 rcvd_msg;
	
	while(1) {
		rcvd_msg = receive();
		(*event_table[global_pid - 1][rcvd_msg - 1])(rcvd_msg);
		
        global_pid = 0;
	}
}
