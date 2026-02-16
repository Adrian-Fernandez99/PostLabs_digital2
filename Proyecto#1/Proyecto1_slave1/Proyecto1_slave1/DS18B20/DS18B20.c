/*
 * DS18B20.c
 *
 * Created: 2/15/2026 11:02:22 PM
 *  Author: adria
 */ 

// Codigo obtenido con ayuda del Datasheet y ChatGPT

#include "DS18B20.h"

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t DS_Reset(void)
{
	uint8_t presence;

	DDRD |= (1<<PORTD2);       // salida
	PORTD &= ~(1<<PORTD2);     // LOW
	_delay_us(480);

	DDRD &= ~(1<<PORTD2);      // entrada
	_delay_us(70);

	presence = !(PIND & (1<<PORTD2));

	_delay_us(410);
	
	return presence;
}

void DS_WriteBit(uint8_t bit)
{
	DDRD |= (1<<PORTD2);
	PORTD &= ~(1<<PORTD2);
	_delay_us(2);

	if(bit)
	{
		DDRD &= ~(1<<PORTD2);	
	}

	_delay_us(60);
	DDRD &= ~(1<<PORTD2);
}

uint8_t DS_ReadBit(void)
{
	uint8_t bit;

	DDRD |= (1<<PORTD2);
	PORTD &= ~(1<<PORTD2);
	_delay_us(2);

	DDRD &= ~(1<<PORTD2);
	_delay_us(15);
	
	if (PIND & (1<<PORTD2))
	{
		bit = 1;
	}
	else
	{
		bit = 0;
	}
	
	_delay_us(50);

	return bit;
}

void DS_WriteByte(uint8_t data)
{
	for(uint8_t i=0; i<8; i++)
	{
		DS_WriteBit(data & 0x01);
		data >>= 1;
	}
}

uint8_t DS_ReadByte(void)
{
	uint8_t data = 0;

	for(uint8_t i=0; i<8; i++)
	{

		if(DS_ReadBit())
		{
			data |= (1 << i);
		}
	}
	return data;
}

// ---------- Configurar resolución a 9 bits ----------

void DS_init(void)
{
	DS_Reset();
	DS_WriteByte(0xCC);   // Skip ROM
	DS_WriteByte(0x4E);   // Write Scratchpad

	DS_WriteByte(0x00);   // TH
	DS_WriteByte(0x00);   // TL
	DS_WriteByte(0x1F);   // Configuración 9 bits (00011111)

	DS_Reset();
	DS_WriteByte(0xCC);
	DS_WriteByte(0x48);   // Copy Scratchpad a EEPROM interna
	_delay_ms(20);
}

// ---------- Leer temperatura (9 bits) ----------

int16_t DS_readTemp(void)
{
	uint8_t lsb, msb;

	DS_Reset();
	DS_WriteByte(0xCC);   // Skip ROM
	DS_WriteByte(0x44);   // Convert T

	_delay_ms(100);       // 9 bits = 94 ms aprox

	DS_Reset();
	DS_WriteByte(0xCC);
	DS_WriteByte(0xBE);   // Read Scratchpad

	lsb = DS_ReadByte();
	msb = DS_ReadByte();

	return (msb << 8) | lsb;
}
