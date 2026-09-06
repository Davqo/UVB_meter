/*
 * LCD.c
 *
 * Created: 23.05.2025 9:12:58
 *  Author: pesek
 */ 

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include "LCD.h"
#include "timer.h"

void configure_LCD_pins()
{
	DDRB |= (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB4) | (1 << PORTB0); // SEG10, SEG13, SEG11, SEG12, SEG22
	DDRD |= (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7) | (1 << PORTD1); // SEG20, SEG23, SEG21, SEG32
	DDRC |=	(1 << PORTC0) | (1 << PORTC2) | (1 << PORTC3); // SEG30, SEG33, SEG31
	
	PORTB &= ~(1 << PORTB1);
	PORTB &= ~(1 << PORTB3);
	PORTB &= ~(1 << PORTB4);
	PORTB &= ~(1 << PORTB2);

	PORTD &= ~(1 << PORTD5);
	PORTD &= ~(1 << PORTD7);
	PORTB &= ~(1 << PORTB0);
	PORTD &= ~(1 << PORTD6);

	PORTC &= ~(1 << PORTC0);
	PORTC &= ~(1 << PORTC3);
	PORTD &= ~(1 << PORTD1);
	PORTC &= ~(1 << PORTC2);
}

void LCD_getBCD(uint16_t num, uint8_t* hundreds, uint8_t* tens, uint8_t* units)
{
	if (num > 999)
		num = 999;
	
	*hundreds = num/100;
	num = num - (*hundreds)*100;
	*tens = num/10;
	num = num - (*tens)*10;
	*units = num;
}

void LCD_write(uint16_t num)
{
	uint8_t hundreds = 0;
	uint8_t tens = 0;
	uint8_t units = 0;
	
	LCD_getBCD(num, &hundreds, &tens, &units);
	LCD_writeSEG1(hundreds);
	timerDelay2(1000000);
	LCD_writeSEG1(tens);
	timerDelay2(1000000);
	LCD_writeSEG1(units);
	
	LCD_writeSEG2(units);
	LCD_writeSEG3(units);
}

void LCD_writeSEG1(uint8_t bcdIn)
{
	if (bcdIn > 9)
		bcdIn = 9;
	
	uint8_t bcd0 = bcdIn & 1;
	uint8_t bcd1 = (bcdIn & 2) >> 1;
	uint8_t bcd2 = (bcdIn & 4) >> 2;
	uint8_t bcd3 = (bcdIn & 8) >> 3;
	
	(bcd0 == 1) ? (PORTB |= 1 << PORTB1) : (PORTB &= ~(1 << PORTB1));
	(bcd1 == 1) ? (PORTB |= 1 << PORTB3) : (PORTB &= ~(1 << PORTB3));
	(bcd2 == 1) ? (PORTB |= 1 << PORTB4) : (PORTB &= ~(1 << PORTB4));
	(bcd3 == 1) ? (PORTB |= 1 << PORTB2) : (PORTB &= ~(1 << PORTB2));
}

void LCD_writeSEG2(uint8_t bcdIn)
{
	if (bcdIn > 9)
	bcdIn = 9;
	
	uint8_t bcd0 = bcdIn & 1;
	uint8_t bcd1 = (bcdIn & 2) >> 1;
	uint8_t bcd2 = (bcdIn & 4) >> 2;
	uint8_t bcd3 = (bcdIn & 8) >> 3;
	
	(bcd0 == 1) ? (PORTD |= 1 << PORTD5) : (PORTD &= ~(1 << PORTD5));
	(bcd1 == 1) ? (PORTD |= 1 << PORTD7) : (PORTD &= ~(1 << PORTD7));
	(bcd2 == 1) ? (PORTB |= 1 << PORTB0) : (PORTB &= ~(1 << PORTB0));
	(bcd3 == 1) ? (PORTD |= 1 << PORTD6) : (PORTD &= ~(1 << PORTD6));
}

void LCD_writeSEG3(uint8_t bcdIn)
{
	if (bcdIn > 9)
	bcdIn = 9;
	
	uint8_t bcd0 = bcdIn & 1;
	uint8_t bcd1 = (bcdIn & 2) >> 1;
	uint8_t bcd2 = (bcdIn & 4) >> 2;
	uint8_t bcd3 = (bcdIn & 8) >> 3;
	
	(bcd0 == 1) ? (PORTC |= 1 << PORTC0) : (PORTC &= ~(1 << PORTC0));
	(bcd1 == 1) ? (PORTC |= 1 << PORTC3) : (PORTC &= ~(1 << PORTC3));
	(bcd2 == 1) ? (PORTD |= 1 << PORTD1) : (PORTD &= ~(1 << PORTD1));
	(bcd3 == 1) ? (PORTC |= 1 << PORTC2) : (PORTC &= ~(1 << PORTC2));
}

void LCD_sleep_routine() {
	DDRB &= ~((1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB4) | (1 << PORTB0)); // SEG10, SEG13, SEG11, SEG12, SEG22
	DDRD &= ~((1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7) | (1 << PORTD1)); // SEG20, SEG23, SEG21, SEG32
	DDRC &=	~((1 << PORTC0) | (1 << PORTC2) | (1 << PORTC3)); // SEG30, SEG33, SEG31
	
	PORTB |= (1 << PORTB1);
	PORTB |= (1 << PORTB3);
	PORTB |= (1 << PORTB4);
	PORTB |= (1 << PORTB2);

	PORTD |= (1 << PORTD5);
	PORTD |= (1 << PORTD7);
	PORTB |= (1 << PORTB0);
	PORTD |= (1 << PORTD6);

	PORTC |= (1 << PORTC0);
	PORTC |= (1 << PORTC3);
	PORTD |= (1 << PORTD1);
	PORTC |= (1 << PORTC2);
}
