/*
 * USART.h
 *
 * Created: 1/28/2026 12:16:00 AM
 *  Author: adria
 */ 


#ifndef USART_H_
#define USART_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void UART_init();
void write_char(uint8_t direct, char caracter);
void write_str(char* texto);


#endif /* USART_H_ */