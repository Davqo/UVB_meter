/*
 * LCD.h
 *
 * Created: 23.05.2025 9:13:07
 *  Author: pesek
 */ 


#ifndef LCD_H_
#define LCD_H_

#pragma once

#define SEG10 PORTB1
#define SEG13 PORTB2
#define SEG11 PORTB3
#define SEG12 PORTB4

#define SEG20 PORTD5
#define SEG23 PORTD6
#define SEG21 PORTD7
#define SEG22 PORTB0

#define SEG30 PORTC0
#define SEG33 PORTC2
#define SEG31 PORTC3
#define SEG32 PORTD1


void configure_LCD_pins();

void LCD_getBCD(uint16_t num, uint8_t* hundreds, uint8_t* tens, uint8_t* units);

void LCD_write(uint16_t num);

void LCD_writeSEG1(uint8_t bcdIn);

void LCD_writeSEG2(uint8_t bcdIn);

void LCD_writeSEG3(uint8_t bcdIn);

void LCD_sleep_routine();

#endif /* LCD_H_ */