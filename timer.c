/*
 * timer.c
 *
 * Created: 25.02.2024 14:26:44
 *  Author: pesek
 */ 

#include <avr/io.h>
#include "timer.h"
#include <stdint.h>
#include <avr/interrupt.h>

volatile uint16_t timer1overflowCount = 0;
//ukol1
void busyDelay(uint32_t us)
{
	us -= 2;							   //  vsechny instrukce pred a za for loopem zaberou 2us
	NOP();								   //  before NOP = 4x LDI + CALL + 2x MOVW + 2x SBIW + 2x SBC + RJMP = 4 + 4 + 2 + 4 + 2 + 4 = 18 steps + 7 steps(navrat z fce) = 25 steps => 7 NOP()
	NOP();								   //  NOP = 1 step
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	//  2x NOP => cely pocet us
	for(; us!=0; --us){					   //  for init = RJMP + 2x SBIW + 2x SBC + 2x CPC = 2 + 4 + 2 + 2 = 10 steps
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
	}
}			// RET = 4 steps


//ukol2
void timerDelay(uint32_t us)
{
	TCNT1 = 0;
	
	us = (us*2) & ~((uint32_t)1 << 31);				// vynasobeni 2, vynulovani bitu 32
	OCR1AH = (us>>8) & 0xFF;					// 9-16 bit
	OCR1AL = us & 0xFF;					// prvnich 8 bitu
	uint16_t overflowCount = (us >> 16) & 0xFFFF;	// poslednich 16 bitu
	
	TIFR1 = (1<<TOV1) | (1<<OCF1A);
	TCCR1B = (1<<CS11);

	while(1)
	{
		if(overflowCount != 0)
		{
			if((TIFR1 & (1<<TOV1)) == 1)				// overflow
			{
				--overflowCount;
				TCNT1 = (uint16_t)0;
				TIFR1 = (1<<TOV1) | (1<<OCF1A);
			}
		}
		else
		{
			if((TIFR1 & (1<<OCF1A)) != 0)				// dopocitani
			{
				TCCR1B = 0;
				TIFR1 = 1<<OCF1A;
				break;
			}
		}
	}
}

//ukol3
void timerInit()
{
	TCNT1 = 0;
	TIFR1 = (1<<TOV1) | (1<<OCF1A);
	TCCR1B = (1<<CS11);
	sei();
	TIMSK1 |= 1<<TOIE1;
}


uint32_t getTime()
{
	return ((uint32_t)TCNT1 + (uint32_t)timer1overflowCount*(0xFFFF));  // pro 16MHz clock -> ((uint32_t)TCNT1 + (uint32_t)timer1overflowCount*(0xFFFF))/2;
}

void timerDelay2(uint32_t us)
{
	uint32_t startTime = getTime();

	while(getTime() < startTime + us)
	{
		NOP();
	}
}

//ISR(TIMER1_OVF_vect)
//{
//	++timer1overflowCount;
//}