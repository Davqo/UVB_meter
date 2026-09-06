/*
 * AS7331.c
 *
 * Created: 20.06.2024 19:29:15
 *  Author: pesek
 */ 

#include "AS7331.h"
#include "I2C.h"
#include "UART.h"
#include "LCD.h"
#include <avr/io.h>
#include "power_saving.h"

uint8_t OSR_state;
uint8_t CREG1_state;
uint8_t CREG3_state;
uint8_t AS7331_ready_flag = 0;
uint16_t AS7331_UVB_raw = 0;
float AS7331_UVB_meas = 0;
uint8_t AS7331_start_flag = 0;
uint8_t start_interrupt_count = 0;

void configure_rdy_syn_pins()
{
	DDRD |= 1 << DDD1;
	PORTD |= 1 << PORTD1;
	DDRD &= ~(1 << DDD2);
	PORTD &= ~(1 << PORTD2);
	
	EIMSK |= (1<<INT0);						// interrupt enable on pin 18 
	EICRA |= (1<<ISC01)|(1<<ISC00);			// interrupt on rising edge of signal
}

void configure_rdy_pin()
{
	DDRC &= ~(1 << DDC3);
	PORTC |= (1 << PORTC3);
	
	PCMSK1 |= 1 << PCINT9;
}

int8_t AS7331_configure_SYN()																	/////////// NOT FUNCTIONAL !!!!!!!!!!!!!!!!!
{	
	I2C_start(AS7331_I2C_address, 0);										// OSR konfigurace
	I2C_Write(OSR);
	I2C_Write((~(1 << SS) & (1 << PD) & ~(1 << SW_RES)) | (1 << DOS1));
	I2C_Stop();
	//UART_write((~(1 << SS) & (1 << PD) & ~(1 << SW_RES)) | (1 << DOS2));
	
	I2C_start(AS7331_I2C_address, 0);										// CREG1 konfigurace
	I2C_Write(CREG1);
	I2C_Write((uint8_t)(1 << GAIN0));
	I2C_Stop();
	
	I2C_start(AS7331_I2C_address, 0);										// CREG3 konfigurace
	I2C_Write(CREG3);
	I2C_Write((uint8_t)((1 << MMODE1) | (1 << SB)));
	I2C_Stop();
	
	I2C_start(AS7331_I2C_address, 0);										// BREAK konfigurace
	I2C_Write(BREAK);
	I2C_Write((uint8_t)(0xFF));
	I2C_Stop();
	
	AS7331_read_OSR();
	I2C_start(AS7331_I2C_address, 0);										// OSR konfigurace
	I2C_Write(OSR);
	I2C_Write(OSR_state | (1 << SS) | (1 << PD) | (1 << DOS1) | (1 << DOS0));
	//UART_write(OSR_state | (1 << SS) | (1 << PD) & ~(1 << DOS2) | (1 << DOS1) | (1 << DOS0));
	I2C_Stop();
	
	return 1;
}

int8_t AS7331_configure_CMD()
{
	I2C_start(AS7331_I2C_address, 0);										// OSR konfigurace
	I2C_Write(OSR);
	I2C_Write((uint8_t)(1 << DOS1));
	I2C_Stop();
	//UART_write((~(1 << SS) & (1 << PD) & ~(1 << SW_RES)) | (1 << DOS2));
	
	I2C_start(AS7331_I2C_address, 0);										// CREG1 konfigurace
	I2C_Write(CREG1);
	I2C_Write((uint8_t)(1 << GAIN1));
	I2C_Stop();
	
	I2C_start(AS7331_I2C_address, 0);										// CREG3 konfigurace
	I2C_Write(CREG3);
	I2C_Write((uint8_t)((1 << MMODE0)));
	I2C_Stop();
	
	I2C_start(AS7331_I2C_address, 0);										// OSR konfigurace
	I2C_Write(OSR);
	I2C_Write((uint8_t)((1 << DOS1) | (1 << DOS0) | (1 << PD)));
	I2C_Stop();
	
	return 1;
}

uint8_t AS7331_read_OSR()
{
	I2C_start(AS7331_I2C_address, 0);
	//UART_write(0);
	I2C_Write(OSR);
	//UART_write(1);
	I2C_repeated_start(AS7331_I2C_address, 1);
	//UART_write(2);
	OSR_state = I2C_Read_Nack();
	//UART_write(3);
	I2C_Stop();
	UART_write(OSR_state);
	
	return OSR_state;
}

uint8_t AS7331_read_CREG1()
{
	I2C_start(AS7331_I2C_address, 0);
	I2C_Write(CREG1);
	I2C_repeated_start(AS7331_I2C_address, 1);
	CREG1_state = I2C_Read_Nack();
	I2C_Stop();
	UART_write(CREG1_state);
	
	return CREG1_state;
}

uint8_t AS7331_read_CREG3()
{
	I2C_start(AS7331_I2C_address, 0);
	I2C_Write(CREG3);
	I2C_repeated_start(AS7331_I2C_address, 1);
	CREG3_state = I2C_Read_Nack();
	I2C_Stop();
	UART_write(CREG3_state);
	
	return CREG3_state;
}

void AS7331_CMD_measurement_UVB()
{	
	I2C_start(AS7331_I2C_address, 0);
	I2C_Write(OSR);
	I2C_Write((uint8_t)((1 << DOS1) | (1 << DOS0) | (1 << PD) | (1 << SS)));
	I2C_Stop();
}

uint16_t AS7331_read_UVB()
{	
	uint8_t byte1 = 0;
	uint8_t byte2 = 0;
	uint16_t measurement = 0;
	I2C_start(AS7331_I2C_address, 0);
	I2C_Write(MRES2);
	I2C_repeated_start(AS7331_I2C_address, 1);
	byte1 = I2C_Read_Ack();
	byte2 = I2C_Read_Nack();
	measurement = (uint16_t)byte1;
	
	measurement |= ((uint16_t)(byte2 << 8));
	I2C_Stop();
	
	AS7331_UVB_raw = measurement;
	return measurement;
}

uint16_t AS7331_get_UVB_from_measurement(uint16_t measurement)
{
	uint32_t calc = ((uint32_t)measurement * UVB_FSR) / 1024;
	return (uint16_t)calc;
}

void AS7331_after_measurement_routine()
{
	AS7331_ready_flag = 0;
	AS7331_read_UVB();
	//AS7331_UVB_meas = AS7331_get_UVB_from_measurement(AS7331_UVB_raw);
	//fprintf(&mystdout, "raw: %u\n", AS7331_UVB_raw);
	//fprintf(&mystdout, "irradiance: %u uW/cm2\n", );
	program_state = 0;
	PCICR &= ~(1<<PCIE1);						// ready interrupt disable
	EIMSK |= (1<<INT0);							// START button interrupt enable
}