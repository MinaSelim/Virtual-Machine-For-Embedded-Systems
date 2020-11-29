#ifndef BSL_OUTPUT_WINDOWS_H
#define BSL_OUTPUT_WINDOWS_H

#include "_stdtype.h"

void bsl_Init_Communication(void);
void bsl_COut_PutB(bool b);
void bsl_COut_PutC(char c);
void bsl_COut_PutS(const char* s);
void bsl_COut_PutI(i32  i);
void bsl_COut_PutU(u32  u);
void bsl_COut_PutX(u32  u);
void bsl_COut_PutN(void);

#endif // !BSL_OUTPUT_WINDOWS_H
