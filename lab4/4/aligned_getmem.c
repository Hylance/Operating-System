/* aligned_getmem.c - aligned_getmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  aligned_getmem  -  Allocate heap storage, returning lowest word address
 *                       on an aligned memory boundary.
 *------------------------------------------------------------------------
 */
int badalignments[5] = {0, 3, 5, 6 ,7};
int i;

char  	*aligned_getmem(
	  uint32	nbytes,		/* Size of memory requested	*/
	  uint32	alignment	/* Alignment in bytes 		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover, *next;

	mask = disable();
	
	for (i = 0; i < 5; i++) {
		if (alignment == badalignments[i]) {
			restore(mask);
			return (char *)SYSERR;
		}
	}
	if (nbytes <= 0 || alignment <= 0) {
		restore(mask);
		return (char *)SYSERR;
	}
	if (nbytes < 8) {
		nbytes = 8;
	}
	if (alignment < 8){
		alignment = 8;
	}
	if (nbytes % alignment != 0) {
		restore(mask);
		return (char *)SYSERR;
	}
	
	if (alignment & (alignment - 1)){
		restore(mask);
		return (char *)SYSERR;
	}

	prev = &memlist;
	curr = memlist.mnext;
	while (curr != NULL) {			

		if (curr->mlength == nbytes &&(uint32)(curr) % alignment == 0) {
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)(curr);

		} else if (curr->mlength > nbytes && (uint32)(curr) % alignment == 0) { 
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)(curr);
		} else if ( curr->mlength > nbytes) {
			int extra = (uint32)(curr) % alignment;
			int need = alignment - extra;	
			int left = curr->mlength - need;
			if ( left < nbytes){			
				prev = curr;
				curr = curr->mnext;
			} else if( left == nbytes){
				next = (struct memblk *)((uint32) curr + need);	
				curr->mlength -= left;
				memlist.mlength -= nbytes;
				restore(mask);
				return (char *)(next);
			} else if( left > nbytes) {
				leftover = (struct memblk *)((uint32) curr + need + nbytes);
				next = (struct memblk *)((uint32) curr + need);
				leftover->mnext = curr->mnext;
				curr->mnext = leftover;
				leftover->mlength = curr->mlength - need - nbytes;
				curr->mlength = need;
				memlist.mlength -= nbytes;
				restore(mask);	
				return (char *) (next);
			}
		}
		else {			
			prev = curr;
			curr = curr->mnext;
		}

	}
	restore(mask);
	return (char *)SYSERR;

}
