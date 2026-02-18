/*
 * PWM.c
 *
 * Created: 2/17/2026 11:00:47 PM
 *  Author: adria
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "PWM.h"

void PWM_init()
{
	DDRB |= (1 << PINB1);  // D9 y D10 como salida

	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);    // Modo Fast PWM 14, TOP = ICR1 y prescaler = 8

	ICR1 = 39999;  // Setear Top como 20ms
}

uint16_t map_servo(uint16_t ADC_var)
{
	uint16_t PWM_var = (ADC_var * 2000UL) / 15;
	return PWM_var;
}
