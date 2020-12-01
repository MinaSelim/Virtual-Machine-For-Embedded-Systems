#ifdef __AVR_ATmega328P__
#include <avr/io.h>
#include "bsl_loader.h"
#include "hal_out.h"
#include "_stdtype.h"

#define SIZE_STATIC_MEMORY (1024)

u8 mem[SIZE_STATIC_MEMORY];

u8* bsl_load_Program(int argc, char** argv)
{
	// Busy wait until the first RX comes in (which 
	while (!(UCSR0A & (1 << RXC0)));
	char size1 = UDR0;

	VMOut_PutS("size1:");
	VMOut_PutX(size1);

	// Busy wait until the first RX comes in (which
	while (!(UCSR0A & (1 << RXC0)));
	char size2 = UDR0;

	VMOut_PutN();
	VMOut_PutS("size2:");
	VMOut_PutX(size2);

	// Define total size of the program
	u16 size = (u16)((size1 << 8) | size2);

	VMOut_PutN();
	VMOut_PutU(size);
	VMOut_PutN();

	int bytesRead = 0;

	//if (size < SIZE_STATIC_MEMORY)
	{
		while (bytesRead < size)
		{
			// Busy wait until next byte comes in
			while (!(UCSR0A & (1 << RXC0)));

			// Read byte that came in
			mem[bytesRead++] = UDR0;

			VMOut_PutC(mem[bytesRead - 1]);
		}
	}

	return mem;
}

u8* bsl_unload_Program(void)
{
	int i;
	for (i = 0; i < SIZE_STATIC_MEMORY; i++) 
	{
		mem[i] = 0x00;
	}
}


#endif
