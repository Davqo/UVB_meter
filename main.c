/*
 * UVB_meter.c
 *
 * Created: 17.06.2024 19:10:29
 * Author : pesek
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdio.h>
//#include "UART.h"
#include "I2C.h"
#include "timer.h"
#include "AS7331.h"
#include "power_saving.h"
#include "LCD.h"
#include "PWM.h"
#include "ADC.h"
#include <avr/interrupt.h>
//#include <avr/sleep.h>

void main(void)
{
	DDRE |= 1 << DDRE0;
	sei();
	ADC_init();
	ADC_start();
	while(ADC_conversion_done == 0)
	MOS_off();
	ADC_check_batt_voltage();
	if (batt_voltage_OK == 0) {
		sleep_configure();
		SLEEP();
	}
	PWM_init();
	timerInit();
	configure_LCD_pins();
	I2C_init();
	//UART_Init(38400);
	AS7331_configure_CMD();
	program_state = 0;
	configure_rdy_pin();
	sleep_configure();
	timerDelay2(1000);
	sleep_routine();
	uint16_t num = 666; 

    while (1) {
		//PORTE |= 1 << PORTE0;
		if (AS7331_start_flag == 1) {
			AS7331_start_flag = 0;
			AS7331_CMD_measurement_UVB();
			PCICR |= (1<<PCIE1);						// ready interrupt enable
		}
		
		if (AS7331_ready_flag == 1)	{
			AS7331_ready_flag = 0;
			AS7331_after_measurement_routine();
			timerDelay2(1000);
			LCD_write(AS7331_get_UVB_from_measurement(AS7331_UVB_raw));
			timerDelay2(1000000);
			PORTE &= ~(1 << PORTE0);
			//PORTE |= 1 << PORTE0;
			sleep_routine();
		}
		
    }
}

