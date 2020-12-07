#ifdef __AVR_ATmega328P__
#include <avr/io.h>
#include "_stdtype.h"

char bsl_readByte() 
{
	while (!(UCSR0A & (1 << RXC0)));
	u8 byte = UDR0;
	return byte;
}

#endif