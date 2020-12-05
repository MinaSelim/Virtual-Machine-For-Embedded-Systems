#ifdef __AVR_ATmega328P__
#include "bsl_io_reg.h"
#include <avr/io.h>

#define B 1
#define C 2
#define D 3


u32 bsl_IOReg_Read(u32 ioreg)
{
	u8* value = (u8*)ioreg;
	return *value;
}

void bsl_IOReg_Write(u32 ioreg, u32 value)
{
	u8* reg = (u8*)ioreg;
	*reg = value;
}
#endif