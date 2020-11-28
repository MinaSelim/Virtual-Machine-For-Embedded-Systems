#ifdef HOST

#include "bsl_output.h"
#include <stdio.h>

void bsl_Init_Communication(void){}

void bsl_COut_PutB(bool b)
{
	printf("%s", b ? "true" : "false");
}

void bsl_COut_PutC(char c)
{
	printf("%c", c);
}

void bsl_COut_PutS(const char* s)
{
	printf("%s", s);
}

void bsl_COut_PutI(i32 i)
{
	printf("%ld", i);
}

void bsl_COut_PutU(u32 u)
{
	printf("%lu", u);
}

void bsl_COut_PutX(u32 u)
{
	printf("%08lX", u);
}

void bsl_COut_PutN(void)
{
	printf("\n");
}


#endif
