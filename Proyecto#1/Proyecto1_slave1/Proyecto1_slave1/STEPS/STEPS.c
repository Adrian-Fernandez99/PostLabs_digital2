/*
 * STEPS.c
 *
 * Created: 2/18/2026 2:45:24 AM
 *  Author: adria
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "STEPS.h"

const uint8_t secuencia_step[8] =
{
	0b0001,
	0b0011,
	0b0010,
	0b0110,
	0b0100,
	0b1100,
	0b1000,
	0b1001
};

uint8_t step_i = 0;

void step_init(void)
{
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3);
	PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3));
}

void mov_step(uint8_t direccion)
{
	if (direccion == 0)  // forward
	{
		step_i = step_i + 1;
		if (step_i >= 8)
		{
			step_i = 0;
		}
	}
	else                 // backward
	{
		if (step_i == 0)
		{
			step_i = 7;
		}
		step_i = step_i - 1;
	}

	PORTB = (PORTB & 0xF0) | secuencia_step[step_i];
}