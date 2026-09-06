/*
 * utilities.c
 *
 * Created: 10.07.2025 11:55:16
 *  Author: pesek
 */ 

#include "utilities.h"
#include <avr/io.h>

void MOS_init() {
	DDRE |= 1 << DDRE2;
	PORTE &= ~(1 << PORTE2);
}

void MOS_on() {
	PORTE |= (1 << PORTE2);
}

void MOS_off() {
	PORTE &= ~(1 << PORTE2);
}
