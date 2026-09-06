/*
 * AS7331.h
 *
 * Created: 20.06.2024 19:29:06
 *  Author: pesek
 */ 


#ifndef AS7331_H_
#define AS7331_H_

#include <stdint.h>

#define AS7331_f_clk 1024000UL

#define AS7331_I2C_address 0b1110100

#define UVB_FSR 756	// uw/cm2

#define OSR 0x0
#define AGEN 0x2
#define CREG1 0x6
#define CREG2 0x7
#define CREG3 0x8
#define BREAK 0x9
#define EDGES 0xA
#define OPTREG 0xB

// MEASUREMENT state addresses
#define OSR_STATUS 0x0
#define TEMP 0x1
#define MRES1 0x2
#define MRES2 0x3
#define MRES3 0x4
#define OUTCONVL 0x5
#define OUTCONVH 0x6

// OSR bits
#define SS 7
#define PD 6
#define SW_RES 3
#define DOS2 2
#define DOS1 1
#define DOS0 0

// AGEN bits
#define DEVID 7
#define MUT 3

// CREG1 bits
#define GAIN3 7
#define GAIN2 6
#define GAIN1 5
#define GAIN0 4
#define TIME 3

// CREG2 bits
#define EN_TM 6
#define EN_DIV 3
#define DIV 2

// CREG3 bits
#define MMODE1 7
#define MMODE0 6
#define SB 4
#define RDYOD 3
#define CCLK1 1
#define CCLK0 0

//OPTREG bits
#define INIT_IDX 0

uint8_t OSR_state;

uint8_t CREG1_state;

uint8_t CREG3_state;

uint8_t AS7331_ready_flag;

uint16_t AS7331_UVB_raw;

float AS7331_UVB_meas;

uint8_t AS7331_start_flag;

uint8_t start_interrupt_count;

int8_t AS7331_configure_SYN();

int8_t AS7331_configure_CMD();

uint8_t AS7331_read_OSR();

void configure_rdy_syn_pins();

void configure_rdy_pin();

uint16_t AS7331_read_UVB();

uint8_t AS7331_read_CREG1();

uint8_t AS7331_read_CREG3();

void AS7331_CMD_measurement_UVB();

uint16_t AS7331_get_UVB_from_measurement(uint16_t measurement);

void AS7331_after_measurement_routine();

// po zapojeni napajeni je v POWDOWN stavu
// nastavit OSR:PD do 0
// pak nakonfigurovat
// nastavit OSR:DOS do hodnoty takove, ktera odpovida modu mereni ktery chci
// nastavit OSR:SS do 1 pro zacatek mereni


// KONFIGURACE PRO SYNS MODE
// nastavit CREG1:TIME na vhodnou hodnotu (doba AD prevodu)						369nW/cm2 - 0b
// nastavit CREG1:GAIN na vhodnou hodnotu (zesileni kanalu A/B/C)				1024x - 0001b
// nastavit CREG3:MMODE na 10b pro SYNS mode
// nastavit CREG3:CCLK na 00b pro 1.024MHz clock
// nastavit CREG3:SB do 1b pro jeste nizsi spotrebu energie

// KONFIGURACE PRO CMD MODE
// nastavit CREG1:TIME na vhodnou hodnotu (doba AD prevodu)						369nW/cm2 - 0b
// nastavit CREG1:GAIN na vhodnou hodnotu (zesileni kanalu A/B/C)				1024x - 0001b
// nastavit CREG3:MMODE na 01b pro CMD mode
// nastavit CREG3:CCLK na 00b pro 1.024MHz clock


// MERENI
// mereni zacne detekci sestupne hrany na SYN pinu
// po skonceni mereni nastavit OSD:PD do 1 pro power down stav
// ...

// CASOVANI
// TstartSB - cas pro zacnuti mereni po standby stau			= 5us
// TstartPD - cas pro zacnuti mereni po power down stavu		= 2ms
// Tconv    - doba AD prevodu									= Nclk*Tclk   ...   Nclk je zavisly na CREG1:TIME
// Tbreak   - nastavena minimalni pauza mezi merenimi			= zavisi na BREAK registru

// 2ms + 5us + 1/(1024*AS7331_f_clk) + 2040us = 4.045ms => max f_vz = 250Hz - staci 2Hz

#endif /* AS7331_H_ */