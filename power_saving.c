/*
 * sleep_configure.c
 *
 * Created: 27.08.2024 10:52:59
 *  Author: pesek
 */ 

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "power_saving.h"
#include "PWM.h"
#include "LCD.h"

uint8_t program_state = 1;		// 0: asleep, 1: awake

void power_save_on_start()
{
	ADCSRA &= ~(1 << ADEN);					// disable ADC
	ACSR &= ~(1 << ACD);					// disable analog comparator
	// mozna vypnout i BOD... Je mozny ze vypne uz pri 2.9V podle datasheetu, takze nutno ozkouset -- vypina se v bootloaderu
	
}

void sleep_configure()
{
	//sleep_bod_disable();				// Tahle funkce se bude hodit az vse pobezi na 3V
	SMCR |= (1 << SM1) | (1 << SE);		// power down mode
	PCICR &= ~(1<<PCIE1);
	
	DDRD &= ~(1 << DDD2);
	PORTD &= 1 << PORTD2;
	
	EIMSK |= (1<<INT0);						// interrupt enable
	EICRA |= (1<<ISC01);			// interrupt on rising edge of signal
}

void sleep_routine() {
	cli();
	LCD_sleep_routine();
	PWM_sleep_routine();
	
	//wdt_reset();
	//MCUSR &= ~(1<<WDRF);
	//WDTCSR |= (1<<WDCE) | (1<<WDE);
	//WDTCSR = 0x00;
	
	EIMSK |= (1<<INT0);
	sei();
	sleep_enable();
	sleep_bod_disable();
	SLEEP();
	wakeup_routine();
}

void wakeup_routine() {
	sleep_disable();
	cli();
	//wdt_reset();
	//WDTCSR |= (1 << WDCE) | (1 << WDE);
	//WDTCSR = (1 << WDIE) | (1 << WDP2) | (1 << WDP1);
	EIMSK &= ~(1<<INT0);			// disable START button interrupt
	PWM_init();
	configure_LCD_pins();
	sei();
}
