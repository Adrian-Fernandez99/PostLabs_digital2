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
		ULTRA_Measure();
		distancia = ULTRA_GetDistance();
		
// 		servo = ((distancia * 200UL / 255) + 25);
// 		OCR1A = servo;
		
		write_str("\n distancia: ");
		
		write_str("|| ");
		write_char(1, (distancia/100) + '0');
		write_char(1, ((distancia/10) %10) +'0');
		write_char(1, (distancia%10) +'0');
		
		if(buffer == 'D')
		{
			PORTB |= (1<<PORTB5);
			buffer = 0;
		}

		_delay_ms(100);
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

// Interrupt routines
ISR(TWI_vect)
{
	uint8_t estado = TWSR & 0xFC;
	switch(estado)
	{
		case 0x60:
		case 0x70:
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
			break;
		
		case 0x80:
		case 0x90:
			buffer = TWDR;
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
			break;
		
		// Slave transmite datos
		case 0xA8:
		case 0xB8:
			TWDR = distancia;
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
			break;
		
		case 0xC0:
		case 0xC8:
		TWCR = 0;
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
			break;
		
		case 0xA0:
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
			break;
		
		// Error
		default:
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
			break;
	}
}
