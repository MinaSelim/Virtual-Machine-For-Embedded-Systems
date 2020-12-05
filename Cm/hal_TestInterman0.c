/* hal_TestInterman0.c -- Test #0 - for Interrupt Management - sdi/ri/di/ei
//
// Copyright (C) 1985-2020 by Michel de Champlain
//
*/
#if defined(TEST) && defined(TEST_TASK6)

#include "hal.h"      // Hal_Init()
#include "hal_Out.h"  // All VMOut_Put*
#include <avr/io.h>

#include "bsl_interrupt.h"
#include "hal_interrupt.h"

// Interrupt Bit is the I-Bit (Bit 7) of SREG (AVR Status Register).
// When I-Bit is cleared (0) all interrupts are disabled.
// When I-Bit is set  (to 1) all interrupts are enabled.
// Instruction SEI SEts   I-Bit to 1 which enable interrupt.
// Instruction CLI CLears I-Bit to 0 which disable interrupts.
// Ref: Atmel 8-bit ATmega328P Datasheet - Section 7.3
#define InterruptBit     ((u8)0x80)

#define Enabled  "[e]"
#define Disabled "[d]"

u32 GetStatusReg(void);

int main(void ) {
    u16 saveStatus;

    Hal_Init();

    VMOut_PutS("Test #0 - Interrupt Manager\n");

    // Legend:
    //
    // <EI>        means enable interrupts: call hal_Interrupt_Enable().
    // <DI>        means disable interrupts: call hal_Interrupt_Disable().
    // <SDI>       means save and disable interrupts: call hal_Interrupt_SaveAndDisable().
    // <RI>        means restore interrupts: call hal_Interrupt_Restore().
    // I[e]        means I-Bit enabled.
    // I[d]        means I-Bit disabled.
    // SI[x]       means save I-Bit status x.

    VMOut_PutS("OnEntry: I[d] | <EI>I[e]<DI>I[d] | <SDI>SI[d]I[d]<RI>I[d] | <EI>I[e]<SDI>SI[e]I[d]<RI>I[e] | OnExit: I[e]\n");

    VMOut_PutS("OnEntry: I");
    VMOut_PutS( (GetStatusReg() & InterruptBit) ? Enabled : Disabled);

    // Test a typical sequence of EI/DI:
    VMOut_PutS(" | ");
    VMOut_PutS("<EI>");
    hal_Interrupt_Enable();

    VMOut_PutS("I");
    VMOut_PutS( (GetStatusReg() & InterruptBit) ? Enabled : Disabled);

    VMOut_PutS("<DI>");
    hal_Interrupt_Disable();

    VMOut_PutS("I");
    VMOut_PutS( (GetStatusReg() & InterruptBit) ? Enabled : Disabled);

    // Test a sequence of SDI/RI when all interrupts are disabled on entry:
    VMOut_PutS(" | ");
    VMOut_PutS("<SDI>");
    saveStatus = hal_Interrupt_SaveAndDisable();

    VMOut_PutS("SI");
    VMOut_PutS( (saveStatus & InterruptBit) ? Enabled : Disabled);
    VMOut_PutS("I");
    VMOut_PutS( (GetStatusReg() & InterruptBit) ? Enabled : Disabled);

    VMOut_PutS("<RI>");
    hal_Interrupt_Restore(saveStatus);

    VMOut_PutS("I");
    VMOut_PutS( (GetStatusReg() & InterruptBit) ? Enabled : Disabled);


    // Test a sequence of SDI/RI when all interrupts are enabled on entry:
    VMOut_PutS(" | ");

    VMOut_PutS("<EI>");
    hal_Interrupt_Enable();

    VMOut_PutS("I");
    VMOut_PutS( (GetStatusReg() & InterruptBit) ? Enabled : Disabled);

    VMOut_PutS("<SDI>");
    saveStatus = hal_Interrupt_SaveAndDisable();

    VMOut_PutS("SI");
    VMOut_PutS( (saveStatus & InterruptBit) ? Enabled : Disabled);
    VMOut_PutS("I");
    VMOut_PutS( (GetStatusReg() & InterruptBit) ? Enabled : Disabled);

    VMOut_PutS("<RI>");
    hal_Interrupt_Restore(saveStatus);

    VMOut_PutS("I");
    VMOut_PutS( (GetStatusReg() & InterruptBit) ? Enabled : Disabled);

    VMOut_PutS(" | ");
    VMOut_PutS("OnExit: ");
    VMOut_PutS("I");
    VMOut_PutS( (GetStatusReg() & InterruptBit) ? Enabled : Disabled);
    VMOut_PutN();

    while(1) { }
    return 0;
}

u32 GetStatusReg() 
{
    return SREG;
}

#endif