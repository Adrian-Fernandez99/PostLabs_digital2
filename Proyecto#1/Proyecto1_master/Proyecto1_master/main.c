/*
 * Proyecto1_master.c
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
#include "USART/USART.h"

#define AHT10_HUM   0x38
#define SLAVE_TEMP   0x10
#define SLAVE_ULTRA   0x15

#define AHT10_HUM_R (0x38 << 1) | 0x01
#define AHT10_HUM_W (0x38 << 1) & 0b11111110

#define slave1R (0x10 << 1) | 0x01
#define slave1W (0x10 << 1) & 0b11111110

uint8_t tp;

uint8_t data_hum[6];
uint32_t hum_val = 0;
uint8_t humedad = 0;

uint8_t buffer_temp = 0;
uint16_t TEMP_val;

void refreshPORT(uint8_t valor);
void init();

int main(void)
{
	cli();
	init();
	UART_init();
	I2C_Master_Init(100000, 1);
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		
		PORTB |= (1<<PORTB5);
		
		tp = I2C_Master_Start();
		if (!tp) return;
		// Comunicación con el sensor de temperatura I2C
		if (!I2C_Master_Write(AHT10_HUM_W))
		{
			I2C_Master_Stop();
			return;
		}
		
		I2C_Master_Write(0xAC);
		I2C_Master_Write(0x33);
		I2C_Master_Write(0x00);
		_delay_ms(40);
		
		if (!I2C_Master_RepeatedStart())
		{
			I2C_Master_Stop();
			return;
		}
		
		if (!I2C_Master_Write(AHT10_HUM_R))
		{
			I2C_Master_Stop();
			return;
		}
		
		for (uint8_t i = 0; i < 6; i++)
		{
			I2C_Master_Read(&data_hum[i], (i < 5));
		}
		
		hum_val = 
			((uint32_t)data_hum[1] << 12) |
			((uint32_t)data_hum[2] << 4)  |
			((uint32_t)data_hum[3] >> 4);
		
		humedad = (hum_val / 1048576.0) * 100.0;		// Fórmula del datasheet
		
		I2C_Master_Stop();
		
		PORTB &= ~(1<<PORTB5);

		write_char(1, '\n');
		write_str("Humedad:");
		write_char(1, humedad);
		_delay_ms(100);

		// Comunicación con el esclavo de temperatura
		PORTB |= (1<<PORTB5);
		write_str("\n A ");
		tp = I2C_Master_Start();
		write_str("\n B ");
		if (!tp) return;
		write_str("\n C ");
		if (!I2C_Master_Write(slave1W))
		{
			I2C_Master_Stop();
			return;
		}
		write_str("\n D ");
		I2C_Master_Write('R');
		write_str("\n E ");
		if (!I2C_Master_RepeatedStart())
		{
			I2C_Master_Stop();
			return;
		}
		write_str("\n F ");
		if (!I2C_Master_Write(slave1R))
		{
			I2C_Master_Stop();
			return;
		}
		write_str("\n G ");
		I2C_Master_Read(&buffer_temp, 0);
		write_str("\n H ");
		I2C_Master_Stop();
		
		PORTB &= ~(1<<PORTB5);
		
		TEMP_val = buffer_temp;
		
		write_str("\n temp: ");
		
		write_str("|| ");
		write_char(1, (TEMP_val/100) + '0');
		write_char(1, ((TEMP_val/10) %10) +'0');
		write_char(1, (TEMP_val%10) +'0');
		
		_delay_ms(100);
    }
}

void init()
{
	DDRD |= ((1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7));
	DDRB |= ((1<<DDB0) | (1<<DDB1));
	
	DDRB |= (1<<DDB5);
	PORTB &= ~(1<<PORTB5);
	
	PORTB &= ~((1<<PORTB0) | (1<<PORTB1));
	PORTD &= ~((1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7));

}

void refreshPORT(uint8_t valor)
{
	if(valor & 0b10000000)
	{
		PORTB |= (1<<PORTB1);
	}
	else
	{
		PORTB &= ~(1<<PORTB1);
	}
	if(valor & 0b01000000)
	{
		PORTB |= (1<<PORTB0);
	}
	else
	{
		PORTB &= ~(1<<PORTB0);
	}
	if(valor & 0b00100000)
	{
		PORTD |= (1<<PORTD7);
	}
	else
	{
		PORTD &= ~(1<<PORTD7);
	}
	if(valor & 0b00010000)
	{
		PORTD |= (1<<PORTD6);
	}
	else
	{
		PORTD &= ~(1<<PORTD6);
	}
	if(valor & 0b00001000)
	{
		PORTD |= (1<<PORTD5);
	}
	else
	{
		PORTD &= ~(1<<PORTD5);
	}
	if(valor & 0b00000100)
	{
		PORTD |= (1<<PORTD4);
	}
	else
	{
		PORTD &= ~(1<<PORTD4);
	}
	if(valor & 0b00000010)
	{
		PORTD |= (1<<PORTD3);
	}
	else
	{
		PORTD &= ~(1<<PORTD3);
	}
	if(valor & 0b00000001)
	{
		PORTD |= (1<<PORTD2);
	}
	else
	{
		PORTD &= ~(1<<PORTD2);
	}
}