#ifdef HOST

#include "bsl_output.h"
#include <stdio.h>

void bsl_Init_Communication(void){}

void bsl_COut_PutC(char c)
{
	printf("%c", c);
}

#endif
