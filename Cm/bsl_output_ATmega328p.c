
#ifdef __AVR_ATmega328P__

#define BAUD9600 (207)


#include "bsl_output.h"
#include <avr/io.h>

void bsl_Init_Communication() {
	// Setup UART

	// Double transmission speed
	UCSR0A = (1 << U2X0);

	// Set baud rate for 9600, 207 value taken from datasheet
	UBRR0H = (BAUD9600 >> 8);
	UBRR0L = (BAUD9600);

	// Set frame format, 8 data bits, 1 stop bit
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

	// Enable receiver and transmitter
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

	// Read the UART register to prepare it for first use (might get optimized away)
	char trash = UDR0;
}


void bsl_COut_PutC(char c)
{
	while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
	UDR0 = c;                    // Send the data to the TX buffer
}


#endif
