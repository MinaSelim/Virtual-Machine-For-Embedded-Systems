#ifndef  HAL_IO_REG_H
#define HAL_IO_REG_H

#include "_stdtype.h"

u32 hal_IOReg_Read(u32 ioreg);
void hal_IOReg_Write(u32 ioreg, u32 value);

#endif // ! BSL_IO_REG_H
