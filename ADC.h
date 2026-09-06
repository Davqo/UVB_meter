/*
 * ADC.h
 *
 * Created: 10.07.2025 8:29:52
 *  Author: pesek
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#define BATTERY_VOLTAGE_MIN 2.75f

extern uint8_t ADC_conversion_done;
extern uint8_t batt_voltage_OK;

void ADC_init();

void ADC_start();

uint16_t ADC_read();

float ADC_compute_voltage(uint16_t ADC_result);

void ADC_check_batt_voltage();

void ADC_disable();

#endif /* ADC_H_ */