/*
 * ADC.c
 *
 * Created: 10.07.2025 8:29:36
 *  Author: pesek
 */ 

#include "ADC.h"
#include <avr/io.h>


uint8_t ADC_conversion_done = 0;
uint8_t batt_voltage_OK = 0;


void ADC_init()
{
	DIDR0 = 0b01111111;																	// disable digital input buffer of all ADC inputs except ADC7
	PRR0 &= ~(1 << PRADC);																// disable power reduction -> can enable AD conversion
	ADMUX = (1 << REFS1) | (1 << REFS0) | (uint8_t)(0x07);								// set reference voltage source(internal 1.1V) | set MUX bits in ADMUX to select ADC7
	ADCSRA = (1 << ADEN) | (1<<ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);		// set ADEN in ADCSRA to enable A/D conversion | set interrupt enable | set clock prescaler (62.5kHz)
}

void ADC_start()
{
	ADCSRA |= 1 << ADSC;
}

uint16_t ADC_read()
{
	return ADC;
}

float ADC_compute_voltage(uint16_t ADC_result)
{
	const float V_max = 1.1;
	const uint16_t AD_max = 1023;
	const uint8_t  V_divider_factor = 3;
	
	float out = ADC_result*V_max/AD_max*V_divider_factor;
	return out;
}

void ADC_check_batt_voltage()
{
	uint16_t ADC_result = ADC_read();
	float batt_voltage = ADC_compute_voltage(ADC_result);
	batt_voltage_OK = (batt_voltage <= BATTERY_VOLTAGE_MIN) ? 0:1;
	ADC_disable();
}



void ADC_disable()
{
	// disable ADC for power saving
	ADCSRA &= ~(1 << ADEN);
	DIDR0 = 0b11111111;
	PRR0 |= (1 << PRADC);
}
