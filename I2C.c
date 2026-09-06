/*
 * I2C.c
 *
 * Created: 21.03.2024 17:45:22
 *  Author: pesek
 */ 

#define F_CPU 8000000UL

#include "UART.h"
#include "I2C.h"
#include "LCD.h"	
#include <avr/io.h>
#include <stdint.h>

#define I2C_PRESCALER 1
#define I2C_FREQ 400000

void I2C_init()
{	
	TWSR0 &= ~((1 << TWPS0) | (1 << TWPS1));
	uint8_t a = 0;
	uint32_t n = (F_CPU - 16*I2C_FREQ);
	uint32_t d = (2*(uint32_t)I2C_FREQ*I2C_PRESCALER);
	a = (n/d);		// max TWBR = 255
	TWBR0 = a;
}

uint8_t I2C_start(uint8_t write_address, uint8_t RW)			// RW=1 => Read
{
	uint8_t status;
	TWCR0 = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);			// Enable TWI, generate START
	while(!(TWCR0&(1<<TWINT)));									// Wait until TWI finishes its current job
	status=TWSR0 & 0xF8;											// Read TWI status register
	if(status != 0x08)											// Check weather START transmitted or not
	{
		//LCD_write(1);
		return 0;
	}
		
		
	TWDR0 = (write_address << 1) | (RW);							// Write slave address and RW bit to data register
	TWCR0 = (1 << TWEN) | (1 << TWINT);							// Enable TWI & clear interrupt flag
	while(!(TWCR0 & (1 << TWINT)));
	
	status = TWSR0 & 0xF8;
	if(status == 0x18)											// Check for SLA+W transmitted & ACK received
	{
		//UART_write(1);
		return 1;												// Return 1 to indicate ack received
	}
		
		
	if(status==0x20)											// Check for SLA+W transmitted & NACK received */
	{
		//UART_write(2);
		return 2;												// Return 2 to indicate NACK received
	}
		
	else
	{
		//UART_write(status);
		return 3;												// Else return 3 to indicate SLA+W failed
	}
		
}


uint8_t I2C_repeated_start(uint8_t read_address, uint8_t RW)
{
	uint8_t status;
	TWCR0 = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);			// Enable TWI, generate start
	while(!(TWCR0 & (1 << TWINT)));								// Wait until TWI finishes its current job
	status = TWSR0 & 0xF8;										// Read TWI status register
	if(status != 0x10)											// Check for repeated start transmitted
		return 0;
		
	TWDR0 = (read_address << 1) | RW;							// Write SLA+R in TWI data register
	TWCR0 = (1 << TWEN) | (1 << TWINT);							// Enable TWI and clear interrupt flag
	while(!(TWCR0 & ( 1<< TWINT)));								// Wait until TWI finishes its current job
	
	status = TWSR0 & 0xF8;										// Read TWI status register
	if(status == 0x40)											// Check for SLA+R transmitted & ACK received
		return 1;
	if(status==0x48)											// Check for SLA+R transmitted & NACK received
		return 2;												// Return 2 to indicate NACK received
	else
		return 3;
}

uint8_t I2C_Write(uint8_t data)
{
	uint8_t status;
	TWDR0 = data;												// Copy data in TWI data register
	TWCR0 = (1 << TWEN) | (1 << TWINT);							// Enable TWI and clear interrupt flag
	while(!(TWCR0 & (1 << TWINT)));								// Wait until TWI finishes its current job
	
	status = TWSR0 & 0xF8;										// Read TWI status register
	//UART_write(status);
	if(status == 0x28){											// Check for data transmitted &ACK received
		return 0;
		LCD_write(1);
	}
	if(status == 0x30)											// Check for data transmitted & NACK received
		return 1;
	else
		return 2;
}

uint8_t I2C_Read_Ack()
{
	TWCR0 = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);			// Enable TWI, generation of ack
	while(!(TWCR0 & (1 << TWINT)));								// Wait until TWI finishes its current job
	return TWDR0;												// Return received data
}

uint8_t I2C_Read_Nack()
{
	TWCR0 = (1 << TWEN) | (1 << TWINT);							// Enable TWI
	while(!(TWCR0 & (1 << TWINT)));								// Wait until TWI finishes its current job
	return TWDR0;												// Return received data
}

void I2C_Stop()
{
	TWCR0 = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);			// Enable TWI, generate stop
	while(TWCR0 & (1 << TWSTO));									// Wait until stop condition execution
}