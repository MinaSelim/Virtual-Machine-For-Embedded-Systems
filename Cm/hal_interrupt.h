#ifndef HAL_INTERRUPT_H
#define HAL_INTERRUPT_H
#include "_stdtype.h"

void hal_Interrupt_Disable(void);
void hal_Interrupt_Enable(void);
u16 hal_Interrupt_SaveAndDisable(void);
void hal_Interrupt_Restore(u16 flags);
void hal_Interrupt_SetVector(u8 number, u32 handlerAddr);
u32 hal_Interrupt_GetVector(u8 number);

#endif