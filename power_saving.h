/*
 * sleep_configure.h
 *
 * Created: 27.08.2024 10:53:19
 *  Author: pesek
 */ 


#ifndef SLEEP_CONFIGURE_H_
#define SLEEP_CONFIGURE_H_

#pragma once

uint8_t program_state;

#define SLEEP() asm volatile ("sleep")

void power_save_on_start();

void sleep_configure();

void sleep_routine();

void wakeup_routine();
#endif /* SLEEP_CONFIGURE_H_ */