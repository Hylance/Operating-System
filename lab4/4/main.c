/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
	char * yihan;
	yihan = aligned_getmem(0,2);
	kprintf("The memory address I get is: %X\n", (uint32)yihan);
	yihan = aligned_getmem(8,-2);
	kprintf("The memory address I get is: %X\n", (uint32)yihan);
	yihan = aligned_getmem(3,3);
	kprintf("The memory address I get is: %X\n", (uint32)yihan);
	yihan = aligned_getmem(20,2);
	kprintf("The memory address I get is: %X\n", (uint32)yihan);
	yihan = aligned_getmem(31,8);
	kprintf("The memory address I get is: %X\n", (uint32)yihan);
	yihan = aligned_getmem(18,9);
	kprintf("The memory address I get is: %X\n", (uint32)yihan);
	yihan = aligned_getmem(8,4);
	kprintf("The memory address I get is: %X\n", (uint32)yihan);
	yihan = aligned_getmem(32,16);
	kprintf("The memory address I get is: %X\n", (uint32)yihan);
	yihan = aligned_getmem(64,32);
	kprintf("The memory address I get is: %X\n", (uint32)yihan);
	return OK;
}
