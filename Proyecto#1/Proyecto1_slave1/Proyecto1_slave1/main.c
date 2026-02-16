/*
 * Proyecto1_slave.c
 *
 * Created: 2/5/2026 7:18:42 PM
 * Author : adria
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "I2C/I2C.h"
#include "ADC/ADC.h"
#include "USART/USART.h"
#include "DS18B20/DS18B20.h"

#define SlaveAddress 0x10

uint8_t ADC_val = 0;
uint8_t buffer = 0;

uint16_t TEMP_val;

void refreshPORT(uint8_t valor);
void init();

int main(void)
{
	cli();
	init();
	ADC_init();
	DS_init();
	UART_init();
	I2C_Slave_Init(SlaveAddress);
	
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		TEMP_val = DS_readTemp();
		
		write_str("\n temp 1: ");
		
		write_str("|| ");
		write_char(1, (TEMP_val/100) + '0');
		write_char(1, ((TEMP_val/10) %10) +'0');
		write_char(1, (TEMP_val%10) +'0');
		
		TEMP_val = (TEMP_val / 16);
		
		write_str("\n temp: ");
		
		write_str("|| ");
		write_char(1, (TEMP_val/100) + '0');
		write_char(1, ((TEMP_val/10) %10) +'0');
		write_char(1, (TEMP_val%10) +'0');
		
		write_str("\n");
		
		if(buffer == 'R')
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
			TWDR = ADC_val;
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA); 
			break;
			
		case 0xC0:
		case 0xC8:
			TWCR = 0;
			TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWEA); 
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
