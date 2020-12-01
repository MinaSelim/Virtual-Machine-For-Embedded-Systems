
#ifdef __AVR_ATmega328P__

#define BAUD9600 (207)


#include "bsl_output.h"
#include <avr/io.h>
#include <stdio.h>

void putChar(char c);
void putString(const char* str);

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

void bsl_COut_PutB(bool b)
{
	putString(b ? "true" : "false");
}

void bsl_COut_PutC(char c)
{
	putChar(c);
}

void bsl_COut_PutS(const char* s)
{
	putString(s);
}

void bsl_COut_PutI(i32 i)
{
	char printString[14];
	sprintf(printString, "%ld", i);
	putString(printString);
}

void bsl_COut_PutU(u32 u)
{
	char printString[14];
	sprintf(printString, "%lu", u);
	putString(printString);
}

void bsl_COut_PutX(u32 u)
{
	char printString[14];
	sprintf(printString, "%08lX", u);
	putString(printString);
}

void bsl_COut_PutN(void)
{
	putChar('\n');
}


void putString(const char* str)
{
	int i = 0;
	while (str[i])
	{
		putChar(str[i]);
		i++;
	}
}

void putChar(char c)
{
	while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
	UDR0 = c;                    // Send the data to the TX buffer
}


#endif
