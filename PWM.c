/*
 * PWM.c
 *
 * Created: 13.06.2025 12:02:51
 *  Author: pesek
 */ 

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include "PWM.h"

void PWM_init() {
	// f_PWM = f_clk/(256*N) ... N=256/1024 (fclk=8/16Mhz) = 122/61Hz
	TCNT2 = 0;
	DDRD |= (1 << PORTD3);	// set as output for PWM to function
	OCR2B = 127;
	TCCR2A |= (1 << COM2B1)|(1 << WGM21)|(1 << WGM20);	// clear OC2B on compare match, set on bottom
	TCCR2B |= (1 << CS22)|(1 << CS21);//|(1 << CS20);
}

void PWM_sleep_routine() {
	TCCR2B &= ~((1 << CS22)|(1 << CS21));
	DDRD &= ~(1 << PORTD3);
	PORTD |= (1 << PORTD3);
}
