#include "Serial.h"
#include <avr/io.h>
#include "Serial.h"

/*
  sci_init - Initialize the SCI port
*/
void sci_init(void) {
	UBRR0 = MYUBRR;				// Set baud rate
	UCSR0B |= (1 << TXEN0);		// Turn on transmitter
	UCSR0B |= (1 << RXEN0);		// Turn on recv
	UCSR0C = (3 << UCSZ00);		// Set for asynchronous operation, no parity, one stop bit, 8 data bits

	DDRD |= (1 << DEMUXSELECT); // set demux select port for output
	DDRD |= (1 << MUXSELECT); // set mux select port for output
}

/*
	sci_out - Output a byte to SCI port
*/
void sci_out(unsigned char ch)
{
	while ( (UCSR0A & (1<<UDRE0)) == 0);
	UDR0 = ch; //input to demux is active low, but is inverted in hardware
}

/*
	sci_outs - Print the contents of the character string "s" out the SCI
	port. The string must be terminated by a zero byte.
*/
void sci_outs(unsigned char *s)
{
	unsigned char ch;

	while ((ch = *s++) != (unsigned char) '\0')
		sci_out(ch);
}

/*
serial_in  - Read a byte  from  the  USART0  and  return  it
*/
char sci_in()
{
	int timeout = 0;
	while ( !( UCSR0A & (1 << RXC0)) ) {
			if (timeout > 1000) {
				return '\0';
			}
			timeout++;
	};

	return  UDR0;
}
