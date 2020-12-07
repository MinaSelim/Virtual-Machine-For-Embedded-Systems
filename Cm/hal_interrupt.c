#include "bsl_interrupt.h"
#include "hal_interrupt.h"

void hal_Interrupt_Disable(void) 
{
	bsl_Interrupt_Disable();
}

void hal_Interrupt_Enable(void) 
{
	bsl_Interrupt_Enable();
}

u16 hal_Interrupt_SaveAndDisable(void) 
{
	return bsl_Interrupt_SaveAndDisable();
}

void hal_Interrupt_Restore(u16 flags) 
{
	bsl_Interrupt_Restore(flags);
}

void hal_Interrupt_SetVector(u8 number, u32 handlerAddr)
{
	bsl_Interrupt_SetVector(number, handlerAddr);
}
u32 hal_Interrupt_GetVector(u8 number)
{
	return bsl_Interrupt_GetVector(number);
}
