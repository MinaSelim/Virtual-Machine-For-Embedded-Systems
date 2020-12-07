/* _cout.c - Implementation of a Console for Cm Hardware Abstract Layer for Output Interface.
//
// Copyright (C) 1999-2020 by Michel de Champlain
//
*/

#include "_outdesc.h"
#include "bsl_output.h"
#include <stdio.h>

void putString(const char* str);

void hal_COut_PutB(bool b)
{
	putString(b ? "true" : "false");
}

void hal_COut_PutC(char c)
{
	bsl_COut_PutC(c);
}

void hal_COut_PutS(const char* s)
{
	putString(s);
}

void hal_COut_PutI(i32 i)
{
	char printString[14];
	sprintf(printString, "%ld", i);
	putString(printString);
}

void hal_COut_PutU(u32 u)
{
	char printString[14];
	sprintf(printString, "%lu", u);
	putString(printString);
}

void hal_COut_PutX(u32 u)
{
	char printString[14];
	sprintf(printString, "%08lX", u);
	putString(printString);
}

void hal_COut_PutN(void)
{
	bsl_COut_PutC('\n');
}


void putString(const char* str)
{
	int i = 0;
	while (str[i])
	{
		bsl_COut_PutC(str[i]);
		i++;
	}
}

static IVMOutDesc cout = {
    hal_COut_PutB,
    bsl_COut_PutC,
    hal_COut_PutI,
    hal_COut_PutU,
    hal_COut_PutS,
    hal_COut_PutX,
    hal_COut_PutN
};

IOut Out_GetFactory(const char* whichOne) {
    bsl_Init_Communication();
    whichOne = 0; // To avoid the warning on the unreferenced formal parameter
    return &cout;
}
