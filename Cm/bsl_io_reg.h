#ifndef  BSL_IO_REG_H
#define BSL_IO_REG_H

#include "_stdtype.h"

u32 bsl_IOReg_Read(u32 ioreg);
void bsl_IOReg_Write(u32 ioreg, u32 value);

#endif // ! BSL_IO_REG_H
