#ifdef __AVR_ATmega328P__
#include "bsl_interrupt.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define InterruptBit (1 << 7)

void bsl_Interrupt_Disable(void)
{
	cli();
}

void bsl_Interrupt_Enable(void)
{
	sei();
}

u16 bsl_Interrupt_SaveAndDisable(void)
{
	u8 status = SREG;
	cli();
	return status;
}

void bsl_Interrupt_Restore(u16 flags)
{
	SREG |= (flags & InterruptBit);
}

void bsl_Interrupt_SetVector(u8 number, u32 handlerAddr)
{
	// NOT IMPLEMENTED
}

u32 bsl_Interrupt_GetVector(u8 number)
{
	// NOT IMPLEMENTED
	return 0;
}
#endif