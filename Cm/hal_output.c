/* _cout.c - Implementation of a Console for Cm Hardware Abstract Layer for Output Interface.
//
// Copyright (C) 1999-2020 by Michel de Champlain
//
*/

#include "_outdesc.h"
#include "bsl_output.h"
static IVMOutDesc cout = {
    bsl_COut_PutB,
    bsl_COut_PutC,
    bsl_COut_PutI,
    bsl_COut_PutU,
    bsl_COut_PutS,
    bsl_COut_PutX,
    bsl_COut_PutN
};

IOut Out_GetFactory(const char* whichOne) {
    whichOne = 0; // To avoid the warning on the unreferenced formal parameter
    return &cout;
}
