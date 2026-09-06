/*
 * UART.h
 *
 * Created: 29.02.2024 12:34:16
 *  Author: pesek
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU 8000000UL
#pragma once
#include <stdio.h>

void UART_Init(uint16_t baud);						// initialize UART communication with entered baudrate

uint8_t UART_get(void);								// get incoming message in UART register

void UART_write(uint8_t ch);						// write message into UART register

void loopback();									// receive message sent from debugger back to debugger

FILE mystdout;										// UART stream handle

static int uart_putchar(char c, FILE *stream);		// printf output stream redirection

uint8_t UART_available();

#endif /* UART_H_ */