/*
 * timer.h
 *
 * Created: 25.02.2024 14:33:08
 *  Author: pesek
 */ 

#ifndef TIMER_H_
#define TIMER_H_

#pragma once
#include <stdint.h>

#define NOP() asm volatile ("nop")

volatile uint16_t timer1overflowCount;		// number of overflows of counter	

void busyDelay(uint32_t us);							// busy delay using NOP

void timerDelay(uint32_t us);							// timer using counter, no need to call timerInit()

void timerInit();										// initialize the timer before using timerDelay2 function

uint32_t getTime();										// get time in us from start of counting, the counter is set to 0.5us rozliseni

void timerDelay2(uint32_t us);							// timer using NOP and counter

#endif /* TIMER_H_ */