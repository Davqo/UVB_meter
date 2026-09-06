/*
 * UART.c
 *
 * Created: 29.02.2024 12:34:05
 *  Author: pesek
 */ 


#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include "UART.h"

void UART_Init(uint16_t baud)
{
	/*Set baud rate */
	UBRR0 = (F_CPU/(baud*16UL)) - 1;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
}
	
uint8_t UART_get(void)
{
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
	
}

uint8_t UART_available()
{
	return ( (UCSR0A & (1<<RXC0)) >> RXC0 );
}

void UART_write(uint8_t ch)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = ch;
}

void loopback()
{
	uint8_t ch = UART_get();
	PORTB ^= (1<<5);
	UART_write(ch);
}

FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
	{
		uart_putchar('\r', stream);
	}
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

