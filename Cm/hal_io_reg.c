#include "hal_io_reg.h"
#include "bsl_io_reg.h"


u32 hal_IOReg_Read(u32 ioreg)
{
	return bsl_IOReg_Read(ioreg);
}

void hal_IOReg_Write(u32 ioreg, u32 value)
{
	bsl_IOReg_Write(ioreg, value);
}