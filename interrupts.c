/*
 * interrupts.c
 *
 * Created: 01.07.2024 11:51:20
 *  Author: pesek
 */ 

#include "timer.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "interrupts.h"
#include "AS7331.h"
#include "power_saving.h"
#include <stdio.h>
#include "UART.h"
#include "ADC.h"

ISR(TIMER1_OVF_vect)
{
	++timer1overflowCount;
}

ISR(PCINT1_vect)					// ready pin
{
	AS7331_ready_flag = 1;
}

ISR(INT0_vect)					// button pin
{	
	AS7331_start_flag = 1;
}

ISR(ADC_vect)
{
	ADC_conversion_done = 1;
}