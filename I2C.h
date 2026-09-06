/*
 * I2C.h
 *
 * Created: 21.03.2024 17:45:30
 *  Author: pesek
 */ 


#ifndef I2C_H_
#define I2C_H_

#pragma once
#include <stdint.h>

void I2C_init();

uint8_t I2C_start(uint8_t write_address, uint8_t RW);

uint8_t I2C_repeated_start(uint8_t read_address, uint8_t RW);

uint8_t I2C_Write(uint8_t data);

uint8_t I2C_Read_Ack();

uint8_t I2C_Read_Nack();

void I2C_Stop();

#endif /* I2C_H_ */