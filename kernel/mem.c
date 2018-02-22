
#include <kernel.h>


void poke_b(MEM_ADDR addr, BYTE value)
{
   char* ptr = (char*) addr;
   *ptr = value;
}

void poke_w(MEM_ADDR addr, WORD value)
{
	short* ptr = (short*) addr;
	*ptr = value;
}

void poke_l(MEM_ADDR addr, LONG value)
{
	long* ptr = (long*) addr;
	*ptr = value;
}

BYTE peek_b(MEM_ADDR addr)
{
	char* ptr = (char*) addr;
	char ch = *ptr;
	return ch;
}

WORD peek_w(MEM_ADDR addr)
{
	short* ptr = (short*) addr;
	short ch = *ptr;
	return ch;
}

LONG peek_l(MEM_ADDR addr)
{
	long* ptr = (long*) addr;
	long ch = *ptr;
	return ch;
}




