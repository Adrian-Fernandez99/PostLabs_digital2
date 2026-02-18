/*
 * ULTRA.c
 *
 * Created: 2/12/2026 7:55:05 PM
 * Author : Britany Barillas
 */ 

#define F_CPU 16000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "USART/USART.h"
#include "ULTRA/ULTRA.h"
#include "I2C/I2C.h"

#define SlaveAddress 0x15

uint8_t distancia = 0;
uint8_t buffer = 0;

uint8_t servo = 0;

void init();
void PWM_init();

int main(void)
{
	cli();
	init();
	UART_init();
	ULTRA_init();
	I2C_Slave_Init(SlaveAddress);
	PWM_init();
	sei();

	while(1)
	{
		
	}
}

void init()
{
	DDRB |= (1<<DDB5);
	PORTB &= ~(1<<PORTB5);

}

void PWM_init()
{
	// Timer 1
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);  // Modo Fast PWM 14, TOP = ICR1 y prescaler = 8

	ICR1 = 20000;  // Setear Top como 20ms
	
	// Timer 2
	TCCR2A = (1 << COM2A1) | (1 << COM0B1) | (1 << WGM21) | (1 << WGM20); // Configurar Fast PWM, no-inverting mode
	TCCR2B = (1 << CS22) | (1 << CS20); // Prescaler de

}
