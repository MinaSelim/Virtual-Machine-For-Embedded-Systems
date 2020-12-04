#ifdef __AVR_ATmega328P__
#include "bsl_io_reg.h"
#include <avr/io.h>

#define B 1
#define C 2
#define D 3


u32 bsl_IOReg_Read(u32 ioreg)
{
	switch (ioreg)
	{
		case B:
			DDRB = 0;
			return PORTB;
		case C:
			DDRC = 0;
			return PORTC;
		case D:
			DDRD = 0;
			return PORTD;
		default:
			return 0;
	}

}

void bsl_IOReg_Write(u32 ioreg, u32 value)
{
	switch (ioreg)
	{
	case B:
		DDRB = 0xff;
		PORTB = value;
	case C:
		DDRB = 0xff;
		PORTB = value;
	case D:
		DDRC = 0xff;
		PORTC = value;
	}
}
#endif