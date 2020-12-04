#ifdef __AVR_ATmega328P__
#include "bsl_interrupt.h"
#include <avr/io.h>
#include <avr/interrupt.h>

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
	return 0;
}

void bsl_Interrupt_Restore(u16 flags)
{

}

void bsl_Interrupt_SetVector(u8 number, u32 handlerAddr)
{

}

u32 bsl_Interrupt_GetVector(u8 number)
{
	return 0;
}
#endif